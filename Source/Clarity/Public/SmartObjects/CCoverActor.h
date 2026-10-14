// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "CCoverActor.generated.h"

class UArrowComponent;
class ACAICharacter;
class UBoxComponent;

/**
 * 
 */
UCLASS()
class CLARITY_API ACCoverActor : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	ACCoverActor();

	UPROPERTY(BlueprintReadWrite)
	ACAICharacter* CurrentCharacter;

	/* Facing direction of each side */
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Components")
	TArray<UArrowComponent*> CoverSpots;

	bool bIsCoverAvailable;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Components")
	UBoxComponent* Collider;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
};
