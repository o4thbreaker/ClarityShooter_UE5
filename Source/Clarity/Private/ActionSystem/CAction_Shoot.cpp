// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CAction_Shoot.h"
#include "CBaseCharacter.h"
#include "Weapons/CWeaponBase.h"
#include "Weapons/CBallistics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "CBaseAnimInstance.h"
#include "Sound/SoundCue.h"
#include "Weapons/CWeaponSlotsComponent.h"
#include "ActionSystem/CActionComponent.h"
#include "CGameplayTags.h"
#include "CAttributeComponent.h"
#include "CShooterInterface.h"

static TAutoConsoleVariable<bool> CVarDrawDebugShootLines(TEXT("art.ShootDrawDebug"), false, TEXT("Enable Debug Lines for shooting"), ECVF_Cheat);

void UCAction_Shoot::Initialize(UCActionComponent* NewActionComponent)
{
	Super::Initialize(NewActionComponent);
	OwnerWeaponSlotsComponent = GetActionOwner()->FindComponentByClass<UCWeaponSlotsComponent>();
}

bool UCAction_Shoot::CanStartAction_Implementation(AActor* Instigator)
{
	if (!Super::CanStartAction_Implementation(Instigator)) return false;

	if (!Cast<ICShooterInterface>(GetActionOwner()))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s doesn't implement ICShooterInterface. Aborting Shoot Action."), *GetNameSafe(GetActionOwner()));
		return false;
	}

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

	/// \NOTE: defensive check, WeaponData is guaranteed non-null by WeaponBase invariant, kept for safety margin
	if (!Weapon->GetWeaponData())
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find Weapon Data on Weapon. Aborting Shoot Action."));
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
		StopAction(GetActionOwner());
		return;
	}

	const FTransform SocketTransform = Weapon->GetMesh()->GetSocketTransform(Weapon->BarrelSocketName);

	PlayFireAnimation(Weapon);

	/* we get position and direction of crosshair. if it is player, it will return its origin on screen and direction
	if it is AI, it will return origin of AI's weapon and direction towards target*/
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	/* is crosshair translated successfully */
	bool bIsCrosshairTranslated = false;

	ICShooterInterface* Shooter = Cast<ICShooterInterface>(GetActionOwner());
	if (Shooter)
	{
		bIsCrosshairTranslated = Shooter->GetAimOriginAndDirection(CrosshairWorldPosition, CrosshairWorldDirection);
		Accuracy = Shooter->GetAccuracy();
		bWillMiss = FMath::FRand() > Accuracy;
	}

	if (bIsCrosshairTranslated)
	{
		FHitResult CrosshairHitResult;
		// from crosshair to direction of crosshair
		FVector Start = CrosshairWorldPosition;
		FVector End = Start + (CrosshairWorldDirection * Weapon->GetWeaponData()->ShotRange);
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(GetActionOwner());
		Params.AddIgnoredActor(Weapon);
		// need to consider what part of body we hit
		Params.bReturnPhysicalMaterial = true;

		PerformCrosshairLineTrace(CrosshairHitResult, Start, End, Params);

		/* trace from weapon to hit location */
		FHitResult WeaponHitResult;
		/* get location to pass it to the function*/
		FVector WeaponStart = SocketTransform.GetLocation();

		PerformWeaponLineTrace(Weapon, WeaponHitResult, CrosshairHitResult, WeaponStart, End, Params);

		if (WeaponHitResult.bBlockingHit)
		{
			ProvideDamage(GetActionOwner(), WeaponHitResult.GetActor(), Weapon, WeaponHitResult);
			PlayImpactEffect(Weapon, WeaponHitResult.Location);
		}
	}

	PlayWeaponRecoil(Weapon);
	PlayCameraShake();

	// add blocking tag after the fire
	ActionComponent->ActiveGameplayTags.AddTag(CGameplayTags::FireCooldown);

	FTimerDelegate CooldownDelegate;
	CooldownDelegate.BindUObject(this, &UCAction_Shoot::ClearFireCooldown);

	GetWorld()->GetTimerManager().SetTimer(FireRateCooldownHandle, CooldownDelegate, Weapon->GetWeaponData()->FireRate, false);

	StopAction(GetActionOwner());
}

void UCAction_Shoot::PerformCrosshairLineTrace(FHitResult& CrosshairHitResult, const FVector& Start, const FVector& End, const FCollisionQueryParams& Params)
{
	GetWorld()->LineTraceSingleByChannel(CrosshairHitResult, Start, End, ECollisionChannel::ECC_GameTraceChannel1, Params);

	DrawDebugLineTrace(Start, End, CrosshairHitResult.Location, FColor::Red, FColor::Blue);
}

void UCAction_Shoot::PerformWeaponLineTrace(const ACWeaponBase* Weapon, FHitResult& WeaponHitResult, const FHitResult& CrosshairHitResult, const FVector& Start, const FVector& End, const FCollisionQueryParams& Params)
{
	FCBulletTraceParams BulletTraceParams;
	BulletTraceParams.bWillMiss = bWillMiss;
	BulletTraceParams.CrosshairHitResult = CrosshairHitResult;
	BulletTraceParams.Start = Start;
	BulletTraceParams.End = End;
	BulletTraceParams.MinMissAngle = Weapon->GetWeaponData()->MinMissAngle;
	BulletTraceParams.MaxMissAngle = Weapon->GetWeaponData()->MaxMissAngle;
	BulletTraceParams.ShotRange = Weapon->GetWeaponData()->ShotRange;

	FVector AdjustedEnd = CBallistics::CalculateBulletEndLocation(BulletTraceParams);

	// ECC_GameTraceChannel1 = Bullet
	GetWorld()->LineTraceSingleByChannel(WeaponHitResult, Start, AdjustedEnd, ECollisionChannel::ECC_GameTraceChannel1, Params);

	DrawDebugLineTrace(Start, AdjustedEnd, WeaponHitResult.Location, FColor::Yellow, FColor::Magenta);
}

void UCAction_Shoot::ProvideDamage(AActor* DamageProvider, AActor* Victim, const ACWeaponBase* Weapon, const FHitResult& WeaponHitResult)
{
	if (IsValid(Victim))
	{
		UCAttributeComponent* AttributeComponent = UCAttributeComponent::GetAttributes(Victim);
		if (AttributeComponent)
		{
			ECHitZone HitZone = AttributeComponent->GetHitZoneFromBoneName(WeaponHitResult.BoneName);

			FHealthChangeInfo HealthChangeInfo;
			HealthChangeInfo.HealthDelta = -Weapon->GetWeaponData()->Damage;
			HealthChangeInfo.HitZone = HitZone;
			HealthChangeInfo.Hit = WeaponHitResult;
			HealthChangeInfo.KnockbackForce = Weapon->GetWeaponData()->KnockbackForce;
			HealthChangeInfo.KnockbackTime = Weapon->GetWeaponData()->KnockbackTime;

			AttributeComponent->ApplyHealthChange(DamageProvider, HealthChangeInfo);
		}
	}
}

void UCAction_Shoot::PlayFireAnimation(const ACWeaponBase* Weapon)
{
	if (Weapon && Weapon->GetWeaponData()->FireAnimation)
	{
		UAnimSequence* FireAnimation = Weapon->GetWeaponData()->FireAnimation;
		Weapon->GetMesh()->PlayAnimation(FireAnimation, false);
	}
}

void UCAction_Shoot::PlayImpactEffect(const ACWeaponBase* Weapon, const FVector& ImpactPoint)
{
	if (Weapon && Weapon->GetWeaponData()->ImpactEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Weapon->GetWeaponData()->ImpactEffect, ImpactPoint);
	}
}

void UCAction_Shoot::PlayWeaponRecoil(const ACWeaponBase* Weapon)
{
	ACBaseCharacter* BaseCharacter = GetActionOwner<ACBaseCharacter>();

	if (BaseCharacter && BaseCharacter->GetBaseAnimInstance())
	{
		BaseCharacter->GetBaseAnimInstance()->DoProceduralRecoil(Weapon->GetWeaponData()->RecoilRate);
	}
}

void UCAction_Shoot::PlayCameraShake()
{
	if (!CameraShakeClass) return;

	ACBaseCharacter* BaseCharacter = GetActionOwner<ACBaseCharacter>();
	if (!BaseCharacter) return;
	
	APlayerController* OwnerPlayerController = Cast<APlayerController>(BaseCharacter->GetController());
	if (OwnerPlayerController && OwnerPlayerController->PlayerCameraManager)
	{
		OwnerPlayerController->PlayerCameraManager->StartCameraShake(CameraShakeClass);
	}
}

void UCAction_Shoot::DrawDebugLineTrace(const FVector& Start, const FVector& End, const FVector& HitLocation, const FColor& TraceColor, const FColor& HitColor)
{
	bool bIsDrawDebug = CVarDrawDebugShootLines.GetValueOnGameThread();
	if (bIsDrawDebug)
	{
		DrawDebugLine(GetWorld(), Start, End, TraceColor, false, 2.0f);
		DrawDebugPoint(GetWorld(), HitLocation, 4.0f, HitColor, false, 2.0f);
	}
}

void UCAction_Shoot::ClearFireCooldown()
{
	// remove the blocking tag after the fire cooldown
	ActionComponent->ActiveGameplayTags.RemoveTag(CGameplayTags::FireCooldown);
}
