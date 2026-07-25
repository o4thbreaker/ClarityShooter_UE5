// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CAction_DisarmEnemy.h"
#include "ActionSystem/CActionComponent.h"
#include "CGameplayTags.h"
#include "CShooterInterface.h"
#include "ContextualAnimUtilities.h"
#include "ContextualAnimSceneActorComponent.h"
#include "CTimeDilationSubsystem.h"
#include "CGameplayTags.h"

static TAutoConsoleVariable<bool> CVarDrawDebugDisarmLines(TEXT("art.DisarmDrawDebug"), true, TEXT("Enable Debug Lines for disarming"), ECVF_Cheat);


UCAction_DisarmEnemy::UCAction_DisarmEnemy()
{
	MaxRange = 500.0f;
	SlowMotionTime = 0.5f;
}

void UCAction_DisarmEnemy::Initialize(UCActionComponent* NewActionComponent)
{
	Super::Initialize(NewActionComponent);

	AnimSceneComponent = NewActionComponent->GetOwner()->FindComponentByClass<UContextualAnimSceneActorComponent>();

	if (ensure(AnimSceneComponent))
	{
		AnimSceneComponent->OnLeftSceneDelegate.AddDynamic(this, &UCAction_DisarmEnemy::OnAnimationEnd);
	}
}

bool UCAction_DisarmEnemy::CanStartAction_Implementation(AActor* Instigator)
{
	if (!ensure(ContextualAnimAsset)) return false;
	if (!ensure(AnimSceneComponent)) return false;

	return Super::CanStartAction_Implementation(Instigator);
}

void UCAction_DisarmEnemy::StartAction_Implementation(AActor* Instigator)
{
	FHitResult Hit = GetTraceHitInfo(Instigator);
	AActor* HitActor = Hit.GetActor();

	if (HitActor)
	{
		/// \NOTE: an actual weapon transfer is in Notify_RetrieveWeapon

		// start animation
		if (PlayContextualAnimation(Instigator, HitActor))
		{
			// add Disarming state
			ActionComponent->ActiveGameplayTags.AddTag(CGameplayTags::Disarming);

			// remove the Armed state
			UCActionComponent* VictimActionComponent = UCActionComponent::GetActionComponent(HitActor);
			if (VictimActionComponent)
			{
				VictimActionComponent->ActiveGameplayTags.RemoveTag(CGameplayTags::Armed);
			}

			// start SlowMo
			UCTimeDilationSubsystem* TimeDilationSubsystem = GetWorld()->GetSubsystem<UCTimeDilationSubsystem>();
			if (TimeDilationSubsystem)
			{
				TimeDilationSubsystem->SetSlowMotion(SlowMotionTime);
			}
		}
	}
}

void UCAction_DisarmEnemy::StopAction_Implementation(AActor* Instigator)
{
	/// \WARNING: this can become a problem in future because StopAction will also be called if the action is interrupted

	UCTimeDilationSubsystem* TimeDilationSubsystem = GetWorld()->GetSubsystem<UCTimeDilationSubsystem>();

	if (TimeDilationSubsystem)
	{
		TimeDilationSubsystem->ResetSlowMotion();
	}
	 
	ActionComponent->ActiveGameplayTags.RemoveTag(CGameplayTags::Disarming);
}


FHitResult UCAction_DisarmEnemy::GetTraceHitInfo(AActor* FromActor)
{
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	/* is crosshair translated successfully */
	bool bIsCrosshairTranslated = false;
	ICShooterInterface* Shooter = Cast<ICShooterInterface>(FromActor);

	if (Shooter)
	{
		bIsCrosshairTranslated = Shooter->GetAimOriginAndDirection(CrosshairWorldPosition, CrosshairWorldDirection);
	}

	if (bIsCrosshairTranslated)
	{
		FHitResult Hit;
		FVector Start = CrosshairWorldPosition;
		FVector End = Start + (CrosshairWorldDirection * MaxRange);
		float CapsuleRadius = 30.0f;
		float CapsuleHalfHeight = 60.0f;
		FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(FromActor);

		FCollisionObjectQueryParams ObjectParams;
		ObjectParams.AddObjectTypesToQuery(ECC_Pawn);

		GetWorld()->SweepSingleByObjectType(Hit, Start, End, FQuat::Identity, ObjectParams, CapsuleShape, Params);

		bool bIsDrawDebug = CVarDrawDebugDisarmLines.GetValueOnGameThread();

		if (bIsDrawDebug)
		{
			FColor Color = Hit.bBlockingHit ? FColor::Green : FColor::Red;
			DrawDebugLine(GetWorld(), Start, End, Color, false, 1.0f);
			DrawDebugSphere(GetWorld(), End, 30.0f, 32, Color, false, 1.0f);
		}

		return Hit;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Couldn't translate crosshair to world position and direction. Hit Result is empty"));
	return FHitResult();
}

bool UCAction_DisarmEnemy::PlayContextualAnimation(AActor* Attacker, AActor* Victim)
{
	FContextualAnimSceneBindings AnimBindingResult;

	const TMap<FName, FContextualAnimSceneBindingContext> BindingContextParams =
	{
		{ FName("Attacker"), FContextualAnimSceneBindingContext(Attacker) },
		{ FName("Victim"), FContextualAnimSceneBindingContext(Victim) }
	};

	bool bIsAnimsBinded = UContextualAnimUtilities::BP_CreateContextualAnimSceneBindings(ContextualAnimAsset, BindingContextParams, AnimBindingResult);

	if (bIsAnimsBinded)
	{
		AnimSceneComponent->StartContextualAnimScene(AnimBindingResult);

		return true;
	}

	return false;
}

void UCAction_DisarmEnemy::OnAnimationEnd(UContextualAnimSceneActorComponent* SceneActorComponent)
{
	StopAction_Implementation(GetOwningComponent()->GetOwner());
}
