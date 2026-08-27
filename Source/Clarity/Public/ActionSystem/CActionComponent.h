// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "ActionSystem/CActionSystemTypes.h"
#include "CActionComponent.generated.h"

class UCAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged, UCActionComponent*, OwningComponent, UCAction*, Action);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTagStateChanged, bool, bIsStarted);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLARITY_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/* active tags */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FGameplayTagContainer ActiveGameplayTags;

	/* called when action starts */
	UPROPERTY(BlueprintAssignable, Category = "Action")
	FOnActionStateChanged OnActionStarted;

	/* called when action stops */
	UPROPERTY(BlueprintAssignable, Category = "Action")
	FOnActionStateChanged OnActionStopped;

	UCActionComponent();

	/* finds or adds tag to delegates map */
	FOnTagStateChanged& RegisterGameplayTagEvent(FGameplayTag Tag);

	void NotifyTagsAdded(const FGameplayTagContainer& Tags);
	void NotifyTagsRemoved(const FGameplayTagContainer& Tags);

	/// \TODO: switch to Interface call, temporary solution
	UFUNCTION(BlueprintCallable, Category = "Attrubutes")
	static UCActionComponent* GetActionComponent(AActor* FromActor);

	/* adds action to the component, and starts it if possible */
	UFUNCTION(BlueprintCallable, Category = "Action")
	UCAction* AddAction(AActor* Instigator, TSubclassOf<UCAction> ActionClass);

	/* stops and removes the actions */
	UFUNCTION(BlueprintCallable, Category = "Action")
	void RemoveAction(AActor* Instigator, UCAction* ActionToRemove);

	/* starts action with name provided */
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StartActionByTag(AActor* Instigator, FGameplayTag ActionTag);

	/* stops action with name provided */
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StopActionByTag(AActor* Instigator, FGameplayTag ActionTag);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsInActions(TSubclassOf<UCAction> ActionClassToCheck) const;

	/* sends a gameplay event with context to the action system. be aware, that it is also starts and action */
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool TryStartActionWithContext(const FActionEventData& EventData);

protected:
	/* current set of activatable actions */
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	TArray<UCAction*> Actions;

	/* actions that can be executed from the start */
	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<TSubclassOf<UCAction>> DefaultActions;

	TMap<FGameplayTag, FOnTagStateChanged> SpecificTagDelegates;

	void CancelActionsWithTags(AActor* Instigator, const FGameplayTagContainer& CancelTags, UCAction* ActionToIgnore);

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
