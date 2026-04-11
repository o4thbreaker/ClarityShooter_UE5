// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "GameplayTagContainer.h"

/**
 * 
 */
namespace CGameplayTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ReloadAction);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(FireAction);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(AimAction);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SprintAction);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Reloading);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Firing);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Aiming);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sprinting);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(FireCooldown);
}
