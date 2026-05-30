// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CShooterInterface.h"
#include "Perception/AISightTargetInterface.h"
#include "Clarity.h"
#include "CAICharacter.generated.h"

class UCAttributeComponent;
class UCActionComponent;
class UCWeaponSlotsComponent;
class ACSmartObject;
class UBehaviorTree;
class ACAIController;
class UCHitReactionComponent;

USTRUCT(BlueprintType)
struct FAnimStates
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	bool bIsCrouching = false;

	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	bool bIsCombat = false;

	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	bool bIsShooting = false;
};


UCLASS(Abstract)
class CLARITY_API ACAICharacter : public ACharacter, public ICShooterInterface, public IAISightTargetInterface
{
	GENERATED_BODY()

public:
	ACAICharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	ECCombatRole CombatRole;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	ACSmartObject* SmartObject;

	/// \TODO: better to refactor later due to architectural issues 
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	ACAIController* AIController;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	FAnimStates AnimState;

	/* Function that is used to initialize weapon and such stuff before BT starts 
	(a little bit of crutch to make the ai get the weapon sooner than bt starts) */
	void Initialize();

	bool IsHostile(AActor* Other);

	virtual bool GetAimOriginAndDirection(FVector& OutOrigin, FVector& OutDirection) const override;

	bool GetIsAiming() const;

	FORCEINLINE virtual ECFaction GetFaction() const override { return Faction; }

	FORCEINLINE void SetIsCrouching(bool bNewIsCrouching) { AnimState.bIsCrouching = bNewIsCrouching; }
	FORCEINLINE void SetIsCombat(bool bNewIsCombat) { AnimState.bIsCombat = bNewIsCombat; }
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
	FORCEINLINE UCActionComponent* GetActionComponent() const { return ActionComponent; }
	FORCEINLINE UCWeaponSlotsComponent* GetWeaponSlotsComponent() const { return WeaponSlotsComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCActionComponent* ActionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCWeaponSlotsComponent* WeaponSlotsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCHitReactionComponent* HitReactionComponent;
	
	/* will be run in Controller class */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	FName PerceptionTarget;

	/// \TODO: might be better to refactor to the data asset
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	ECFaction Faction;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	/* Custom view target for perception component */
	virtual bool CanBeSeenFrom(const FVector& ObserverLocation, FHitResult& OutHitResult, const AActor* IgnoreActor = nullptr) const;

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
};
