// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CAction_DisarmEnemy.h"
#include "ActionSystem/CActionComponent.h"
#include "CGameplayTags.h"
#include "Weapons/CWeaponSlotsComponent.h"
#include "CShooterInterface.h"
#include "ContextualAnimUtilities.h"
#include "ContextualAnimSceneActorComponent.h"

UCAction_DisarmEnemy::UCAction_DisarmEnemy()
{
	Range = 500.0f;
}

bool UCAction_DisarmEnemy::CanStartAction_Implementation(AActor* Instigator)
{
	if (!ensure(ContextualAnimAsset)) return false;

	return Super::CanStartAction_Implementation(Instigator);
}

void UCAction_DisarmEnemy::StartAction_Implementation(AActor* Instigator)
{
	FHitResult Hit = GetTraceHitInfo(Instigator);
	AActor* HitActor = Hit.GetActor();

	if (HitActor)
	{
		UCWeaponSlotsComponent* TargetWeaponSlotsComponent = UCWeaponSlotsComponent::GetWeaponSlotsComponent(HitActor);
		if (!TargetWeaponSlotsComponent) return;
		
		UCWeaponSlotsComponent* OwnerWeaponSlotsComponent = UCWeaponSlotsComponent::GetWeaponSlotsComponent(Instigator);
		if (!OwnerWeaponSlotsComponent) return;
		
		PlayContextualAnimation(Instigator, HitActor);

		ACWeaponBase* TargetWeapon = TargetWeaponSlotsComponent->LoseCurrentWeapon();
		OwnerWeaponSlotsComponent->SwitchWeapon(TargetWeapon);
	}
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
		FVector End = Start + (CrosshairWorldDirection * Range);
		float CapsuleRadius = 30.0f;
		float CapsuleHalfHeight = 60.0f;
		FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(FromActor);

		FCollisionObjectQueryParams ObjectParams;
		ObjectParams.AddObjectTypesToQuery(ECC_Pawn);

		GetWorld()->SweepSingleByObjectType(Hit, Start, End, FQuat::Identity, ObjectParams, CapsuleShape, Params);

		FColor Color = Hit.bBlockingHit ? FColor::Green : FColor::Red;
		DrawDebugLine(GetWorld(), Start, End, Color, false, 1.0f);
		DrawDebugSphere(GetWorld(), End, 30.0f, 32, Color, false, 1.0f);

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
		if (UContextualAnimSceneActorComponent* AnimSceneComponent = Attacker->FindComponentByClass<UContextualAnimSceneActorComponent>())
		{
			AnimSceneComponent->StartContextualAnimScene(AnimBindingResult);
			return true;
		}
	}

	return false;
}
