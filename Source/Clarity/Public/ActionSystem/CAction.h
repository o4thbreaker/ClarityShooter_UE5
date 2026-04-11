// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "CAction.generated.h"

class UCActionComponent;
/**
 * 
 */
UCLASS(Blueprintable)
class CLARITY_API UCAction : public UObject
{
	GENERATED_BODY()

public:
	/* tag of the action */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FGameplayTag ActionTag;

	/* tag of the action */
	/// \NOTE: will be replaced with tag
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName; 

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bIsAutoStart;

	void Initialize(UCActionComponent* NewActionComponent);

	/* will be implemented in child classes */
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	/* will be implemented in child classes */
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	/* will be implemented in child classes */
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	UCActionComponent* GetOwningComponent() const { return ActionComponent; }

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const { return bIsRunning; }

	/* returns the world */
	UWorld* GetWorld() const override;

protected:

	UPROPERTY()
	UCActionComponent* ActionComponent;

	/* what tags will be added to the instigator of this action */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FGameplayTagContainer GrantsTags;

	/* action can only start if OWNER has none of this TAGS applied */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FGameplayTagContainer BlockedTags;

	bool bIsRunning;
	
};
