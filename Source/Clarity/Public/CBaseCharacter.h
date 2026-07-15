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

	FORCEINLINE UCBaseAnimInstance* GetBaseAnimInstance() { return BaseAnimInstance; }
	FORCEINLINE UContextualAnimSceneActorComponent* GetContextualAnimComponent() { return ContextualAnimComponent; }

protected:
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

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnWeaponEquiped(UCWeaponSlotsComponent* OwningComp, ACWeaponBase* Weapon);

	UFUNCTION()
	void OnWeaponLost(UCWeaponSlotsComponent* OwningComp);

	UFUNCTION()
	void OnAnimSceneStateSwitch(UContextualAnimSceneActorComponent* SceneActorComponent);
};
