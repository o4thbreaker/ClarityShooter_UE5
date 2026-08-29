// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/CAction.h"
#include "Clarity.h"
#include "CAction_Reload.generated.h"

class ACWeaponBase;
class UCWeaponSlotsComponent;

/**
 * 
 */
UCLASS()
class CLARITY_API UCAction_Reload : public UCAction
{
	GENERATED_BODY()
	
public:
	UCAction_Reload();

	virtual void Initialize(UCActionComponent* NewActionComponent) override;

	virtual bool CanStartAction_Implementation(AActor* Instigator) override;
	virtual void StartAction_Implementation(AActor* Instigator) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	ECWeaponCategory WeaponCategory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	float RefillTime;

	FTimerHandle ReloadTimerHandle;

	UPROPERTY()
	UCWeaponSlotsComponent* WeaponSlotsComponent;
	
private:
	UFUNCTION()
	void ReloadWeapon(AActor* ActorToReload);

	UFUNCTION()
	void PlayReloadMontage(AActor* ActorToReload);

	void PlayWeaponReloadAnimation();
};
