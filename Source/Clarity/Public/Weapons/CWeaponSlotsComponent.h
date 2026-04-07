// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CWeaponSlotsComponent.generated.h"

class ACWeaponBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLARITY_API UCWeaponSlotsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCWeaponSlotsComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<ACWeaponBase> DefaultWeapon;

	ACWeaponBase* CurrentWeapon;

	FName WeaponSocketName;

	virtual void BeginPlay() override;

public:	
	void SpawnWeapon();

	FORCEINLINE ACWeaponBase* GetCurrentWeapon() const { return CurrentWeapon; }
};
