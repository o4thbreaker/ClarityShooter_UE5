// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CAction_Reload.h"
#include "Weapons/CWeaponSlotsComponent.h"
#include "Weapons/CWeaponBase.h"
#include "CGameplayTags.h"
#include "ActionSystem/CActionComponent.h"
#include "GameFramework/Character.h"

UCAction_Reload::UCAction_Reload()
{
	RefillTime = 2.0f;
}

bool UCAction_Reload::CanStartAction_Implementation(AActor* Instigator)
{
	Super::CanStartAction_Implementation(Instigator);

	UCWeaponSlotsComponent* OwnerWeaponSlotsComponent = Instigator->FindComponentByClass<UCWeaponSlotsComponent>();
	if (!IsValid(OwnerWeaponSlotsComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find WeaponSlotsComponent in %s!"), *GetNameSafe(this));
		StopAction_Implementation(Instigator);
		return false;
	}

	Weapon = OwnerWeaponSlotsComponent->GetCurrentWeapon();
	if (!IsValid(Weapon))
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find Weapon in %s!"), *GetNameSafe(this));
		StopAction_Implementation(Instigator);
		return false;
	}

	if (Weapon->GetCurrentAmmoCount() >= Weapon->GetWeaponData()->MagazineSize)
	{
		UE_LOG(LogTemp, Log, TEXT("Can't start reloading due to full magazine. Current ammo: %d"), Weapon->GetCurrentAmmoCount());
		return false;
	}

	return true;
}

void UCAction_Reload::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	PlayReloadMontage(Instigator);
}

void UCAction_Reload::ReloadWeapon(AActor* Instigator)
{
	Weapon->Reload();

	StopAction_Implementation(Instigator);
}

void UCAction_Reload::PlayReloadMontage(AActor* Instigator)
{
	ACharacter* Character = Cast<ACharacter>(Instigator);
	
	if (ensure(Character))
	{
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Couldn't get AnimInstance from Character!"));
			return;
		}

		if (Weapon->GetWeaponData()->ReloadMontage)
		{
			AnimInstance->Montage_Play(Weapon->GetWeaponData()->ReloadMontage);

			if (!GetWorld()->GetTimerManager().IsTimerActive(ReloadTimerHandle))
			{
				FTimerDelegate Delegate;
				Delegate.BindUObject(this, &UCAction_Reload::ReloadWeapon, Instigator);

				GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, Delegate, RefillTime, false);
			}	
		}
	}
}
