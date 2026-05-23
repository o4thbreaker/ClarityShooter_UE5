// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/** Main log category used across the project */
DECLARE_LOG_CATEGORY_EXTERN(LogClarity, Log, All);

/* global enum for AI states */
UENUM(BlueprintType)
enum class ECAIState : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	ManDown		UMETA(DisplayName = "Man Down"),
	Investigate UMETA(DisplayName = "Investigate"),
	LostEnemy	UMETA(DisplayName = "Lost Enemy"),
	Search		UMETA(DisplayName = "Search"),
	Attack		UMETA(DisplayName = "Attack")
};

/* global enum for any possible factions in the game */
UENUM(BlueprintType)
enum class ECFaction : uint8
{
	Enemy 		UMETA(DisplayName = "Enemy"),
	Ally 		UMETA(DisplayName = "Ally"),
	Neutral 	UMETA(DisplayName = "Neutral"),
};

/* global enum for combat state needed to dynamically decide what agent should do */
UENUM(BlueprintType)
enum class ECCombatStatae : uint8
{
	SoftCover 				UMETA(DisplayName = "Soft Cover"),
	ApproachingCover 		UMETA(DisplayName = "Approaching Cover"),
	Flank 					UMETA(DisplayName = "Flank"),
	HoldCover 				UMETA(DisplayName = "Hold Cover"),
	RetreatingToCover 		UMETA(DisplayName = "Retreating to Cover"),
	Evade 					UMETA(DisplayName = "Evade"),
};