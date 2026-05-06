// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CBTAmmoObserver.generated.h"

class UBlackboardComponent;
class ACWeaponBase;

/**
 * Highly cohesive class that observes the ammo count of a weapon and updates the blackboard key accordingly
 * It is made for CBTService_CheckAmmoLevel to keep the NodeMemory updated with the current ammo level of the weapon
 */
UCLASS()
class CLARITY_API UCBTAmmoObserver : public UObject
{
	GENERATED_BODY()
	
public:
	/* instantiated in User's class */
	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;

	/* instantiated in User's class */
	UPROPERTY()
	ACWeaponBase* Weapon;

	/* instantiated in User's class */
	float LowAmmoPercentage;

	/* instantiated in User's class. Could be FBlackboardKeySelector, but I've chosen the lightweight variant */
	FName LowAmmoKeyName;

	UFUNCTION()
	void OnAmmoChanged(int32 NewAmmoCount);
};
