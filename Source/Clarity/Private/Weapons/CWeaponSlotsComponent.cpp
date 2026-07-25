// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CWeaponSlotsComponent.h"
#include "Weapons/CWeaponBase.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/Character.h"
#include "CGameplayTags.h"

UCWeaponSlotsComponent::UCWeaponSlotsComponent()
{
	/// \NOTE: hardcoded to character
	WeaponSocketName = FName(TEXT("RightHandSocket"));
}

UCWeaponSlotsComponent* UCWeaponSlotsComponent::GetWeaponSlotsComponent(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UCWeaponSlotsComponent>(FromActor->GetComponentByClass(UCWeaponSlotsComponent::StaticClass()));
	}

	return nullptr;
}

void UCWeaponSlotsComponent::SwitchWeapon(ACWeaponBase* NewWeapon)
{
	/// \TODO: maybe not lose but stash in the inventory
	LoseCurrentWeapon();
	EquipWeapon(NewWeapon);
}

void UCWeaponSlotsComponent::EquipWeapon(ACWeaponBase* Weapon)
{
	if (CurrentWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't equip weapon because owner has already weapon equipped"));
		return;
	}
	 
	CurrentWeapon = Weapon;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());

	const USkeletalMeshSocket* RightHandSocket = OwnerCharacter->GetMesh()->GetSocketByName(WeaponSocketName);

	if (CurrentWeapon && OwnerCharacter)
	{
		if (RightHandSocket)
		{
			RightHandSocket->AttachActor(Cast<AActor>(CurrentWeapon), OwnerCharacter->GetMesh());
			OnWeaponEquiped.Broadcast(this, Weapon);
		}
	}
}

ACWeaponBase* UCWeaponSlotsComponent::LoseCurrentWeapon()
{
	if (IsValid(CurrentWeapon))
	{
		ACWeaponBase* Weapon = CurrentWeapon;

		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon = nullptr;

		OnWeaponLost.Broadcast(this);

		return Weapon;
	}

	return nullptr;
}

void UCWeaponSlotsComponent::SpawnDefaultWeapon()
{
	ACWeaponBase* Weapon = GetWorld()->SpawnActor<ACWeaponBase>(DefaultWeapon);
	
	EquipWeapon(Weapon);
}