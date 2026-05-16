// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/CSmartObject.h"
#include "CSmartObject_PatrolPath.generated.h"

class USplineComponent;

/**
 * 
 */
UCLASS()
class CLARITY_API ACSmartObject_PatrolPath : public ACSmartObject
{
	GENERATED_BODY()
	
public:
	ACSmartObject_PatrolPath();

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Path")
	USplineComponent* Path;

	/* Array to store the locations */
	TArray<FVector> Locations;

	virtual void BeginPlay() override;

	void GetSplinePoints();

};
