// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "CActionComponent.generated.h"

class UCAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged, UCActionComponent*, OwningComponent, UCAction*, Action);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLARITY_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCActionComponent();

	/* active tags */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FGameplayTagContainer ActiveGameplayTags;

	/* called when action starts */
	UPROPERTY(BlueprintAssignable, Category = "Action")
	FOnActionStateChanged OnActionStarted;

	/* called when action stops */
	UPROPERTY(BlueprintAssignable, Category = "Action")
	FOnActionStateChanged OnActionStopped;

	/* adds action to the component, and starts it if possible */
	UFUNCTION(BlueprintCallable, Category = "Action")
	void AddAction(AActor* Instigator, TSubclassOf<UCAction> ActionClass);

	/* stops and removes the actions */
	UFUNCTION(BlueprintCallable, Category = "Action")
	void RemoveAction(AActor* Instigator, UCAction* ActionToRemove);

	/* starts action with name provided */
	/// \TODO: change to FGameplayTag by creating different script struct
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	/* stops action with name provided */
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StopActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsInActions(TSubclassOf<UCAction> ActionClassToCheck) const;

protected:
	/* active actions */
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	TArray<UCAction*> Actions;

	/* actions that can be executed from the start */
	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<TSubclassOf<UCAction>> DefaultActions;

	virtual void BeginPlay() override;
		
};
