// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAttributeComponent.generated.h"

/* enum for hit zones */
UENUM(BlueprintType)
enum class ECHitZone : uint8
{
	Body 		UMETA(DisplayName = "Body"),
	Head 		UMETA(DisplayName = "Head"),
	Arm 		UMETA(DisplayName = "Arm"),
	Hand		UMETA(DisplayName = "Hand"),
	Foot		UMETA(DisplayName = "Foot"),
};

USTRUCT(BlueprintType)
struct FHealthChangeInfo
{
	GENERATED_BODY()

public:
	float HealthDelta;

	float KnockbackTime;
	float KnockbackForce;

	FHitResult Hit;
	ECHitZone HitZone;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, Instigator, UCAttributeComponent*, OwningComponent, float, NewHealth, FHealthChangeInfo, HealthChangeInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDamage, AActor*, Instigator, UCAttributeComponent*, OwningComponent, float, NewHealth, FHealthChangeInfo, HealthChangeInfo);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLARITY_API UCAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCAttributeComponent();

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	FOnDamage OnDamage;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, FHealthChangeInfo& HeathChangeInfo);

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category="Attrubutes")
	static UCAttributeComponent* GetAttributes(AActor* FromActor);

	ECHitZone GetHitZoneFromBoneName(FName BoneNAme) const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsAlive() const { return Health > 0; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsFullHealth() const { return MaxHealth <= Health; }

	FORCEINLINE float GetHealth() const { return Health; }

	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	bool bShouldLimbDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	TMap<FName, ECHitZone> HitZones;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, FHealthChangeInfo HeathChangeInfo);

	UFUNCTION(Category = "Attributes")
	bool HandleDamage(AActor* InstigatorActor, const FHealthChangeInfo& HeathChangeInfo);

	UFUNCTION()
	float CalculateLimbDamageMultiplier(const ECHitZone& HitZone) const;
};
