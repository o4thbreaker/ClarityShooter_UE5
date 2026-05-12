// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CShooterInterface.h"
#include "CAICharacter.generated.h"

class UCAttributeComponent;
class UCActionComponent;
class UCWeaponSlotsComponent;

UCLASS(Abstract)
class CLARITY_API ACAICharacter : public ACharacter, public ICShooterInterface
{
	GENERATED_BODY()

public:
	ACAICharacter();

	/* Function that is used to initialize weapon and such stuff before BT starts 
	(a little bit of crutch to make the ai get the weapon sooner than bt starts) */
	void Initialize();

	virtual bool GetAimOriginAndDirection(FVector& OutOrigin, FVector& OutDirection) const override;

	FORCEINLINE bool GetIsAiming() const { return bIsAiming; }
	FORCEINLINE void SetIsAiming(bool bNewIsAiming) { bIsAiming = bNewIsAiming; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCActionComponent* ActionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCWeaponSlotsComponent* WeaponSlotsComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Aim")
	bool bIsAiming;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
};
