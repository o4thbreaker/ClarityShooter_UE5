// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/CWeaponData.h"
#include "CWeaponBase.generated.h"

UCLASS()
class CLARITY_API ACWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ACWeaponBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	UCWeaponData* WeaponData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;

public:
	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return WeaponMesh; };
};
