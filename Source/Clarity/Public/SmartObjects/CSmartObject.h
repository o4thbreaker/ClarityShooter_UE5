// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSmartObject.generated.h"

class UBillboardComponent;
class UArrowComponent;
class UBehaviorTree;

UCLASS()
class CLARITY_API ACSmartObject : public AActor
{
	GENERATED_BODY()
	
public:	
	ACSmartObject();

	/* Component to click and select on the level */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Components")
	UBillboardComponent* Billboard;

	/* Direction that AI should face to interact with object */
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Components")
	UArrowComponent* FacingDirection;

	/* Behavior tree to run when AI interacts with this object */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	UBehaviorTree* SubTree;
};
