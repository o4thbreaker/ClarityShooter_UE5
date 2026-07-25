// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/** Main log category used across the project */
DECLARE_LOG_CATEGORY_EXTERN(LogClarity, Log, All);

/* global enum for AI states */
/// \WARNING: please remember that ordering is IMPORTANT for this enum. number = state priority. 
///           0 - the highest priority, 6 - the lowest.
UENUM(BlueprintType)
enum class ECAIState : uint8
{
	Idle	= 6		UMETA(DisplayName = "Idle"),
	ManDown = 0		UMETA(DisplayName = "Man Down"),
	Investigate	= 4	UMETA(DisplayName = "Investigate"),
	LostEnemy	= 3	UMETA(DisplayName = "Lost Enemy"),
	Search	= 5		UMETA(DisplayName = "Search"),
	Attack	= 2		UMETA(DisplayName = "Attack"),
	WeaponLost = 1	UMETA(DisplayName = "WeaponLost")
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
enum class ECCombatState : uint8
{
	SoftCover 				UMETA(DisplayName = "Soft Cover"),
	ApproachingCover 		UMETA(DisplayName = "Approaching Cover"),
	Flank 					UMETA(DisplayName = "Flank"),
	HoldCover 				UMETA(DisplayName = "Hold Cover"),
	RetreatingToCover 		UMETA(DisplayName = "Retreating to Cover"),
	Evade 					UMETA(DisplayName = "Evade"),
	NeedToReload 			UMETA(DisplayName = "Need to Reload"),
};

/* global enum for combat roles to divide behaviour. Probably will be refactored */
UENUM(BlueprintType)
enum class ECCombatRole : uint8
{
	Engager 					UMETA(DisplayName = "Engager"),
	Defender 					UMETA(DisplayName = "Defender"),
	Ambusher 					UMETA(DisplayName = "Ambusher"),
};