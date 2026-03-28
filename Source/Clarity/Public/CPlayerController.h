// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

class UInputMappingContext;
/**
 * Controller to manage input mappings
 */
UCLASS(Abstract)
class CLARITY_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	/** input mapping contexts */
	UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** input mapping context setup */
	virtual void SetupInputComponent() override;
};
