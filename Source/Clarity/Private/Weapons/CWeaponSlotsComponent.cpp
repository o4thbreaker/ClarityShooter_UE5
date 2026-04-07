// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CWeaponSlotsComponent.h"
#include "CWeaponBase.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/Character.h"

UCWeaponSlotsComponent::UCWeaponSlotsComponent()
{
	/// \NOTE: hardcoded to character
	WeaponSocketName = FName(TEXT("RightHandSocket"));
}

void UCWeaponSlotsComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCWeaponSlotsComponent::SpawnWeapon()
{
	CurrentWeapon = GetWorld()->SpawnActor<ACWeaponBase>(DefaultWeapon);

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (CurrentWeapon)
	{
		const USkeletalMeshSocket* RightHandSocket = OwnerCharacter->GetMesh()->GetSocketByName(WeaponSocketName);
		if (RightHandSocket)
		{
			RightHandSocket->AttachActor(Cast<AActor>(CurrentWeapon), OwnerCharacter->GetMesh());
		}
	}
}

