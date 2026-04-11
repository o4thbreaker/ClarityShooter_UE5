// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CAction_Reload.h"
#include "Weapons/CWeaponSlotsComponent.h"
#include "Weapons/CWeaponBase.h"
#include "CPlayerCharacter.h"
#include "CPlayerAnimInstance.h"
#include "CGameplayTags.h"
#include "ActionSystem/CActionComponent.h"

UCAction_Reload::UCAction_Reload()
{
	RefillTime = 2.0f;
}

void UCAction_Reload::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	UCWeaponSlotsComponent* OwnerWeaponSlotsComponent = Instigator->FindComponentByClass<UCWeaponSlotsComponent>();
	if (!IsValid(OwnerWeaponSlotsComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find WeaponSlotsComponent in %s!"), *GetNameSafe(this));
		StopAction_Implementation(Instigator);
		return;
	}

	ACWeaponBase* CurrentWeapon = OwnerWeaponSlotsComponent->GetCurrentWeapon();
	if (!IsValid(CurrentWeapon))
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find CurrentWeapon in %s!"), *GetNameSafe(this));
		StopAction_Implementation(Instigator);
		return;
	}

	if (CurrentWeapon->GetCurrentAmmoCount() < CurrentWeapon->GetWeaponData()->MagazineSize)
	{
		PlayReloadMontage(Instigator, CurrentWeapon);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Stopping the action due to full magazine. Current ammo: %d"), CurrentWeapon->GetCurrentAmmoCount());
		StopAction_Implementation(Instigator);
	} 
}

void UCAction_Reload::ReloadWeapon(AActor* Instigator, ACWeaponBase* Weapon)
{
	Weapon->Reload();

	StopAction_Implementation(Instigator);
}

void UCAction_Reload::PlayReloadMontage(AActor* Instigator, ACWeaponBase* Weapon)
{
	ACPlayerCharacter* PlayerCharacter = Cast<ACPlayerCharacter>(Instigator);

	if (ensure(PlayerCharacter->GetPlayerAnimInstance()))
	{
		if (Weapon->GetWeaponData()->ReloadMontage)
		{
			PlayerCharacter->GetPlayerAnimInstance()->Montage_Play(Weapon->GetWeaponData()->ReloadMontage);

			if (!GetWorld()->GetTimerManager().IsTimerActive(ReloadTimerHandle))
			{
				FTimerDelegate Delegate;
				Delegate.BindUObject(this, &UCAction_Reload::ReloadWeapon, Instigator, Weapon);

				GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, Delegate, RefillTime, false);
			}	
		}
	}
}
