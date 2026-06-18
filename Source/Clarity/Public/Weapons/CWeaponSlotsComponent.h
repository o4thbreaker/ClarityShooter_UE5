// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CWeaponSlotsComponent.generated.h"

class ACWeaponBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponEquiped, UCWeaponSlotsComponent*, OwningComponent, ACWeaponBase*, Weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponLost, UCWeaponSlotsComponent*, OwningComponent);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLARITY_API UCWeaponSlotsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCWeaponSlotsComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<ACWeaponBase> DefaultWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	ACWeaponBase* CurrentWeapon;

	FName WeaponSocketName;
public:
	FOnWeaponEquiped OnWeaponEquiped;
	FOnWeaponLost OnWeaponLost;

	static UCWeaponSlotsComponent* GetWeaponSlotsComponent(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SwitchWeapon(ACWeaponBase* NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon(ACWeaponBase* Weapon);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	ACWeaponBase* LoseCurrentWeapon();

	void SpawnDefaultWeapon();

	FORCEINLINE ACWeaponBase* GetCurrentWeapon() const { return CurrentWeapon; }
};
