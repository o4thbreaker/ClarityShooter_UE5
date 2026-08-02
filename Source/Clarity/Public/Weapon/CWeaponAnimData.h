// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Clarity.h"
#include "CWeaponAnimData.generated.h"

/**
 * data asset to store the weapon animations to dynamically change the skeleton
 */
UCLASS()
class CLARITY_API UCWeaponAnimData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TMap<ECWeaponCategory, UAnimMontage*> ReloadMontages;
};
