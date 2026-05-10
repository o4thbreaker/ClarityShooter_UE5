// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CAction_Shoot.h"
#include "CPlayerCharacter.h"
#include "Weapons/CWeaponBase.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "CPlayerAnimInstance.h"
#include "Sound/SoundCue.h"
#include "Weapons/CWeaponSlotsComponent.h"
#include "ActionSystem/CActionComponent.h"
#include "CGameplayTags.h"
#include "CAttributeComponent.h"
#include "CPlayerController.h"
#include "AI/CAIController.h"

bool UCAction_Shoot::CanStartAction_Implementation(AActor* Instigator)
{
	if (!Super::CanStartAction_Implementation(Instigator)) return false;

	OwnerWeaponSlotsComponent = Instigator->FindComponentByClass<UCWeaponSlotsComponent>();
	if (!IsValid(OwnerWeaponSlotsComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find WeaponSlotsComponent. Aborting Shoot Action."));
		return false;
	}

	Weapon = OwnerWeaponSlotsComponent->GetCurrentWeapon();
	if (!IsValid(Weapon))
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find attached Weapon. Aborting Shoot Action."));
		return false;
	}

	return ActionComponent->ActiveGameplayTags.HasAll(RequiredTags) && Weapon->CanFire();
}

// Fire
void UCAction_Shoot::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	// handle the ammo logic
	if (!Weapon->TryConsumeAmmo())
	{
		StopAction_Implementation(Instigator);
		return;
	}

	PlayFireSound(Instigator);

	const FTransform SocketTransform = Weapon->GetMesh()->GetSocketTransform(Weapon->BarrelSocketName);
	PlayMuzzleFlash(Instigator, SocketTransform);

	// we get position and direction of crosshair. if it is player, it will return its origin on screen and direction
	// if it is AI, it will return origin of AI's weapon and direction towards target
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	bIsCrosshairTranslated = GetFireOriginAndDirection(Instigator, CrosshairWorldPosition, CrosshairWorldDirection);

	if (bIsCrosshairTranslated)
	{
		// from crosshair to direction of crosshair
		FHitResult HitResult;
		FVector Start = CrosshairWorldPosition;
		/// \TODO: fix the magic number
		FVector End = Start + CrosshairWorldDirection * 60000.0f;
		
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Instigator);
		Params.AddIgnoredActor(Weapon);

		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_GameTraceChannel1, Params);

		if (HitResult.bBlockingHit)
		{
			PlayImpactEffect(Instigator, HitResult.Location);

			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);
			DrawDebugPoint(GetWorld(), HitResult.Location, 4.0f, FColor::Blue, false, 2.0f);
		}

		/* trace from weapon to hit location */
		FHitResult WeaponTraceHit;

		const FVector WeaponStart = SocketTransform.GetLocation();
		FVector WeaponEnd = HitResult.bBlockingHit ? HitResult.Location : End;

		// ECC_GameTraceChannel1 = Bullet
		GetWorld()->LineTraceSingleByChannel(WeaponTraceHit, WeaponStart, WeaponEnd, ECollisionChannel::ECC_GameTraceChannel1, Params);
		if (WeaponTraceHit.bBlockingHit)
		{
			DrawDebugLine(GetWorld(), WeaponStart, WeaponEnd, FColor::Yellow, false, 2.0f);
			DrawDebugPoint(GetWorld(), WeaponTraceHit.Location, 4.0f, FColor::Magenta, false, 2.0f);

			AActor* HitActor = WeaponTraceHit.GetActor();
			UE_LOG(LogTemp, Log, TEXT("%s"), *HitActor->GetName());
			if (IsValid(HitActor))
			{
				UCAttributeComponent* AttributeComponent = UCAttributeComponent::GetAttributes(HitActor);
				if (AttributeComponent)
				{
					AttributeComponent->ApplyHealthChange(Instigator, -Weapon->GetWeaponData()->Damage);
				}
			}
		}
	}

	PlayWeaponRecoil(Instigator);

	// add blocking tag after the fire
	ActionComponent->ActiveGameplayTags.AddTag(CGameplayTags::FireCooldown);

	// set timer to handle remove the fire cooldown timer
	FTimerHandle FireRateCooldownHandle;

	// i use lambda because too lazy to make whole new function
	GetWorld()->GetTimerManager().SetTimer(FireRateCooldownHandle, [this]() {ActionComponent->ActiveGameplayTags.RemoveTag(CGameplayTags::FireCooldown); }, 
		Weapon->GetWeaponData()->FireRate, false);

	StopAction(Instigator);
}

void UCAction_Shoot::PlayFireSound(AActor* Instigator)
{
	USoundCue* FiringAudio = Weapon->GetWeaponData()->FiringAudio;

	if (FiringAudio)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FiringAudio, Instigator->GetActorLocation(),
			1.0f, 1.0f, 0.0f, FiringAudio->AttenuationSettings);
	}
}

void UCAction_Shoot::PlayMuzzleFlash(AActor* Instigator, const FTransform SocketTransform)
{
	if (Weapon->GetWeaponData()->MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Weapon->GetWeaponData()->MuzzleFlash, SocketTransform);
	}
}

void UCAction_Shoot::PlayImpactEffect(AActor* Instigator, const FVector& ImpactPoint)
{
	if (Weapon->GetWeaponData()->ImpactEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Weapon->GetWeaponData()->ImpactEffect, ImpactPoint);
	}
}

/// \FIXME: another cast -> poor performance
void UCAction_Shoot::PlayWeaponRecoil(AActor* Instigator)
{
	ACPlayerCharacter* PlayerCharacter = Cast<ACPlayerCharacter>(Instigator);

	if (PlayerCharacter && PlayerCharacter->GetPlayerAnimInstance())
	{
		PlayerCharacter->GetPlayerAnimInstance()->DoProceduralRecoil(1.5f);
	}
}

bool UCAction_Shoot::GetFireOriginAndDirection(AActor* Instigator, FVector& OutOrigin, FVector& OutDirection)
{
	if (Instigator->GetInstigatorController<ACPlayerController>())
	{
		// Player Logic
		return GetCrosshairWorldProperties(OutOrigin, OutDirection);
	}
	else
	{
		//AI Logic
		ACAIController* AIController = Cast<ACAIController>(Instigator->GetInstigatorController());
		if (!AIController)
		{
			UE_LOG(LogTemp, Warning, TEXT("Instigator on Shoot_Action is not a valid ACAIController"));
			return false;
		}

		AActor* TargetActor = AIController->GetCurrentTarget();
		if (TargetActor == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("AI has no target"));
			return false;
		}

		UE_LOG(LogTemp, Log, TEXT("Target for AI is: %s"), *GetNameSafe(TargetActor));

		OutOrigin = Weapon->GetMesh()->GetSocketTransform(Weapon->BarrelSocketName).GetLocation();

		/// \TODO: maybe create a better aiming system for AI
		OutDirection = (TargetActor->GetActorLocation() - OutOrigin).GetSafeNormal();

		return true;
	}
}

bool UCAction_Shoot::GetCrosshairWorldProperties(FVector& WorldPosition, FVector& WorldDirection)
{
	FVector2D ViewportSize;

	// get screen size
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	// calculate crosshair location on screen (in the middle)
	FVector2D CrosshairLocation(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);

	// try to convert data from screen to world and save to WorldPosition and WorldDirection
	return UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), CrosshairLocation, WorldPosition, WorldDirection);
}