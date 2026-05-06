// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CBTAmmoObserver.h"
#include "Weapons/CWeaponBase.h"
#include "BehaviorTree/BlackboardComponent.h"

void UCBTAmmoObserver::OnAmmoChanged(int32 NewAmmoCount)
{
	float CurrentAmmoPercentage = (NewAmmoCount / (float)Weapon->GetWeaponData()->MagazineSize) * 100.0f;

	BlackboardComponent->SetValueAsBool(LowAmmoKeyName, CurrentAmmoPercentage <= LowAmmoPercentage);
}