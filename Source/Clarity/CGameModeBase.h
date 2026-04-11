// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CGameModeBase.generated.h"

/**
 * Base GameMode
 */
UCLASS(Abstract)
class CLARITY_API ACGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ACGameModeBase();

	virtual void OnActorKilled(AActor* VictimActor, AActor* KillerActor);
};
