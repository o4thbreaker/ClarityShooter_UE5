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

	/* name of the action */
	/// \NOTE: DEPRECATED (has been replaced with tag)
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName; 

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bIsAutoStart;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bWantsTick;

	UCAction();

	virtual void Initialize(UCActionComponent* NewActionComponent);

	/* will be implemented in child classes */
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	/* will be implemented in child classes */
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	/* will be implemented in child classes */
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStartAction(AActor* Instigator);

	/* will be implemented in child classes */
	UFUNCTION(Category = "Action")
	virtual void TickAction(float DeltaTime) {};

	UFUNCTION(BlueprintCallable, Category = "Action")
	FORCEINLINE bool IsRunning() const { return bIsRunning; }

	UFUNCTION(BlueprintCallable, Category = "Action")
	FORCEINLINE bool IsStopping() const { return bIsStopping; }

	UFUNCTION(BlueprintCallable, Category = "Action")
	UCActionComponent* GetOwningComponent() const { return ActionComponent; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Action")
	AActor* GetActionOwner() const;

	/// \NOTE: because it as a template function, we have to implement it in the header file
	/* generic override to get smth like ACharacter */
	template<class T>
	T* GetActionOwner() const
	{
		AActor* Owner = GetActionOwner();
		return Owner ? Cast<T>(Owner) : nullptr;
	}


	UFUNCTION(BlueprintCallable, Category = "Action")
	const FGameplayTagContainer& GetCancelActionWithTags() const { return CancelActionsWithTags; }

	/* returns the world */
	UWorld* GetWorld() const override;

protected:

	UPROPERTY()
	UCActionComponent* ActionComponent;

	/* what tags will be added to the instigator of this action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
	FGameplayTagContainer GrantsTags;

	/* action can only start if OWNER has none of this TAGS applied */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
	FGameplayTagContainer BlockedTags;

	/* when this action starts, all actions with those tags will be cancelled */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
	FGameplayTagContainer CancelActionsWithTags;

	bool bIsRunning;

	/* to handle actions with outro */
	bool bIsStopping;
	
};
