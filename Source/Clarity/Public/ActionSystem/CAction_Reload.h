// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/CAction.h"
#include "CAction_Reload.generated.h"

class ACWeaponBase;

/**
 * 
 */
UCLASS()
class CLARITY_API UCAction_Reload : public UCAction
{
	GENERATED_BODY()
	
public:
	UCAction_Reload();

protected:
	FTimerHandle ReloadTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	float RefillTime;

	virtual void StartAction_Implementation(AActor* Instigator) override;

private:
	UFUNCTION()
	void ReloadWeapon(AActor* Instigator, ACWeaponBase* Weapon);

	UFUNCTION()
	void PlayReloadMontage(AActor* Instigator, ACWeaponBase* Weapon);
};
