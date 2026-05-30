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
#include "CShooterInterface.h"
#include "CHitReactionComponent.h"

void UCAction_Shoot::Initialize(UCActionComponent* NewActionComponent)
{
	Super::Initialize(NewActionComponent);
	OwnerWeaponSlotsComponent = Cast<UCWeaponSlotsComponent>(NewActionComponent->GetOwner()->FindComponentByClass<UCWeaponSlotsComponent>());
}

bool UCAction_Shoot::CanStartAction_Implementation(AActor* Instigator)
{
	if (!Super::CanStartAction_Implementation(Instigator)) return false;

	if (!IsValid(OwnerWeaponSlotsComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find WeaponSlotsComponent. Aborting Shoot Action."));
		return false;
	}

	ACWeaponBase* Weapon = OwnerWeaponSlotsComponent->GetCurrentWeapon();
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

	ACWeaponBase* Weapon = OwnerWeaponSlotsComponent->GetCurrentWeapon();

	// handle the ammo logic
	if (!Weapon->TryConsumeAmmo())
	{
		StopAction_Implementation(Instigator);
		return;
	}

	PlayFireSound(Instigator, Weapon);

	const FTransform SocketTransform = Weapon->GetMesh()->GetSocketTransform(Weapon->BarrelSocketName);
	PlayMuzzleFlash(Instigator, Weapon, SocketTransform);

	/* we get position and direction of crosshair. if it is player, it will return its origin on screen and direction
	if it is AI, it will return origin of AI's weapon and direction towards target*/
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	/* is crosshair translated successfully */
	bool bIsCrosshairTranslated = false;

	if (ICShooterInterface* Shooter = Cast<ICShooterInterface>(Instigator))
	{
		bIsCrosshairTranslated = Shooter->GetAimOriginAndDirection(CrosshairWorldPosition, CrosshairWorldDirection);
	}

	if (bIsCrosshairTranslated)
	{
		// from crosshair to direction of crosshair
		FHitResult CrosshairHitResult;
		FVector Start = CrosshairWorldPosition;
		/// \TODO: fix the magic number
		FVector End = Start + CrosshairWorldDirection * 60000.0f;
		
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Instigator);
		Params.AddIgnoredActor(Weapon);
		// need to consider what part of body we hit
		Params.bReturnPhysicalMaterial = true;

		GetWorld()->LineTraceSingleByChannel(CrosshairHitResult, Start, End, ECollisionChannel::ECC_GameTraceChannel1, Params);

		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);
		//DrawDebugPoint(GetWorld(), CrosshairHitResult.Location, 4.0f, FColor::Blue, false, 2.0f);
		
		if (CrosshairHitResult.bBlockingHit)
		{
			PlayImpactEffect(Instigator, Weapon, CrosshairHitResult.Location);
		}

		/* trace from weapon to hit location */
		FHitResult WeaponHitResult;

		const FVector WeaponStart = SocketTransform.GetLocation();

		/// \NOTE: due to floating point precision WeaponEnd can be coincided exactly with the surface point from CrosshairHitResult
		///		   and UE can sometimes not register the intersection
		FVector WeaponEnd = CrosshairHitResult.bBlockingHit ? CrosshairHitResult.Location : End;

		/// \NOTE: so we have to adjust it by small number to make a difference
		FVector WeaponDirection = (WeaponEnd - WeaponStart).GetSafeNormal();
		FVector AdjustedEnd = CrosshairHitResult.bBlockingHit ? CrosshairHitResult.Location + WeaponDirection * 10.0f : End;

		// ECC_GameTraceChannel1 = Bullet
		GetWorld()->LineTraceSingleByChannel(WeaponHitResult, WeaponStart, AdjustedEnd, ECollisionChannel::ECC_GameTraceChannel1, Params);

		DrawDebugLine(GetWorld(), WeaponStart, AdjustedEnd, FColor::Yellow, false, 2.0f);
		DrawDebugPoint(GetWorld(), WeaponHitResult.Location, 4.0f, FColor::Magenta, false, 2.0f);

		if (WeaponHitResult.bBlockingHit)
		{
			AActor* HitActor = WeaponHitResult.GetActor();
			if (IsValid(HitActor))
			{
				UCAttributeComponent* AttributeComponent = UCAttributeComponent::GetAttributes(HitActor);
				if (AttributeComponent)
				{
					AttributeComponent->ApplyHealthChange(Instigator, -Weapon->GetWeaponData()->Damage);

					/// \WARNING:: testing only!!!! refactor later
					UCHitReactionComponent* HitReactionComponent = Cast<UCHitReactionComponent>(HitActor->FindComponentByClass(UCHitReactionComponent::StaticClass()));

					if (HitReactionComponent)
					{
						UE_LOG(LogTemp, Log, TEXT("HitBone to React: %s"), *WeaponHitResult.BoneName.ToString());
						HitReactionComponent->HitReaction(WeaponHitResult);
					}
				}
			}
		}
	}

	PlayWeaponRecoil(Instigator);

	// add blocking tag after the fire
	ActionComponent->ActiveGameplayTags.AddTag(CGameplayTags::FireCooldown);

	// i use lambda because too lazy to make whole new function
	GetWorld()->GetTimerManager().SetTimer(FireRateCooldownHandle, [this]() {ActionComponent->ActiveGameplayTags.RemoveTag(CGameplayTags::FireCooldown); }, 
		Weapon->GetWeaponData()->FireRate, false);

	StopAction(Instigator);
}

void UCAction_Shoot::PlayFireSound(AActor* Instigator, ACWeaponBase* Weapon)
{
	USoundCue* FiringAudio = Weapon->GetWeaponData()->FiringAudio;

	if (FiringAudio)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FiringAudio, Instigator->GetActorLocation(),
			1.0f, 1.0f, 0.0f, FiringAudio->AttenuationSettings);
	}
}

void UCAction_Shoot::PlayMuzzleFlash(AActor* Instigator, ACWeaponBase* Weapon, const FTransform SocketTransform)
{
	if (Weapon->GetWeaponData()->MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Weapon->GetWeaponData()->MuzzleFlash, SocketTransform);
	}
}

void UCAction_Shoot::PlayImpactEffect(AActor* Instigator, ACWeaponBase* Weapon, const FVector& ImpactPoint)
{
	if (Weapon->GetWeaponData()->ImpactEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Weapon->GetWeaponData()->ImpactEffect, ImpactPoint);
	}
}

/// \FIXME: depends on PlayerAnimInstance
void UCAction_Shoot::PlayWeaponRecoil(AActor* Instigator)
{
	ACPlayerCharacter* PlayerCharacter = Cast<ACPlayerCharacter>(Instigator);

	if (PlayerCharacter && PlayerCharacter->GetPlayerAnimInstance())
	{
		PlayerCharacter->GetPlayerAnimInstance()->DoProceduralRecoil(1.5f);
	}
}