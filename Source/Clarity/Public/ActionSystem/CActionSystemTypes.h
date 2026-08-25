#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "CActionSystemTypes.generated.h"

USTRUCT(BlueprintType)
struct FActionEventData
{
	GENERATED_BODY()

public:

	/* must be the same as ActionTag that we want to start */
	UPROPERTY(BlueprintReadWrite, Category = "EventData")
	FGameplayTag EventTag;

	UPROPERTY(BlueprintReadWrite, Category = "EventData")
	AActor* Instigator = nullptr;

	/* polymorphic container to contain any struct */
	UPROPERTY(BlueprintReadWrite, Category = "EventData", meta = (BaseStruct = "/Script/CoreUObject.Struct"))
	FInstancedStruct EventContext;
};