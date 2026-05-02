// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CAICharacter.generated.h"

class UPawnSensingComponent;
class UCAttributeComponent;
class UCActionComponent;
class UCWeaponSlotsComponent;

UCLASS(Abstract)
class CLARITY_API ACAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACAICharacter();

	FORCEINLINE AActor* GetCurrentTarget() const { return CurrentTarget; }
	FORCEINLINE void SetCurrentTarget(AActor* NewTarget) { CurrentTarget = NewTarget; }

	FORCEINLINE bool GetIsAiming() const { return bIsAiming; }
	FORCEINLINE void SetIsAiming(bool NewAiming) { bIsAiming = NewAiming; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCActionComponent* ActionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCWeaponSlotsComponent* WeaponSlotsComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Aim")
	bool bIsAiming;

	UPROPERTY()
	AActor* CurrentTarget;

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	void SetTargetActor(AActor* NewTarget);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

};
