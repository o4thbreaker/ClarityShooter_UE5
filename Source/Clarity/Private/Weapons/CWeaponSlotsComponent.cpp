// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CWeaponSlotsComponent.h"
#include "Weapons/CWeaponBase.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/Character.h"
#include "ActionSystem/CActionComponent.h"
#include "CGameplayTags.h"

UCWeaponSlotsComponent::UCWeaponSlotsComponent()
{
	/// \NOTE: hardcoded to character
	WeaponSocketName = FName(TEXT("RightHandSocket"));
}

void UCWeaponSlotsComponent::BeginPlay()
{
	Super::BeginPlay();
}

UCWeaponSlotsComponent* UCWeaponSlotsComponent::GetWeaponSlotsComponent(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UCWeaponSlotsComponent>(FromActor->GetComponentByClass(UCWeaponSlotsComponent::StaticClass()));
	}

	return nullptr;
}

void UCWeaponSlotsComponent::SpawnWeapon()
{
	CurrentWeapon = GetWorld()->SpawnActor<ACWeaponBase>(DefaultWeapon);

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (CurrentWeapon && OwnerCharacter)
	{
		const USkeletalMeshSocket* RightHandSocket = OwnerCharacter->GetMesh()->GetSocketByName(WeaponSocketName);
		if (RightHandSocket)
		{
			RightHandSocket->AttachActor(Cast<AActor>(CurrentWeapon), OwnerCharacter->GetMesh());
		}
	}
}

