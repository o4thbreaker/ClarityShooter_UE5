// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Clarity.h"
#include "CShooterInterface.h"
#include "CBaseCharacter.generated.h"

class UCActionComponent;
class UCWeaponSlotsComponent;
class UCAttributeComponent;
class ACWeaponBase;
class UContextualAnimSceneActorComponent;
class UMotionWarpingComponent;
class UCBaseAnimInstance;
class UCWeaponAnimData;

UCLASS()
class CLARITY_API ACBaseCharacter : public ACharacter, public ICShooterInterface
{
	GENERATED_BODY()

public:
	ACBaseCharacter();

	/* Empty. Should be overriden in derived classes */
	virtual bool GetAimOriginAndDirection(FVector& OutOrigin, FVector& OutDirection) const override;

	/* Empty. Should be overriden in derived classes */
	virtual ECFaction GetFaction() const override;

	FORCEINLINE virtual float GetAccuracy() const override { return Accuracy; }

	FORCEINLINE UCBaseAnimInstance* GetBaseAnimInstance() const { return BaseAnimInstance; }
	FORCEINLINE UContextualAnimSceneActorComponent* GetContextualAnimComponent() const { return ContextualAnimComponent; }
	FORCEINLINE UCWeaponAnimData* GetWeaponAnimData() const { return WeaponAnimData; }
	FORCEINLINE UCAttributeComponent* GetAttributeComponent() const { return AttributeComponent; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float Accuracy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bIsInContextualAnimMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	UCBaseAnimInstance* BaseAnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCWeaponSlotsComponent* WeaponSlotsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCActionComponent* ActionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UContextualAnimSceneActorComponent* ContextualAnimComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMotionWarpingComponent* MotionWarpingComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UCWeaponAnimData* WeaponAnimData;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnWeaponEquiped(UCWeaponSlotsComponent* OwningComp, ACWeaponBase* Weapon);

	UFUNCTION()
	void OnWeaponLost(UCWeaponSlotsComponent* OwningComp);

	UFUNCTION()
	void OnContextualAnimStateChange(UContextualAnimSceneActorComponent* SceneActorComponent);
};
