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
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(AimCameraAction);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SprintCameraAction);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(DisarmAction);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SwapShoulderAction);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReactionAction);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(DeathAction);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Reloading);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Firing);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Aiming);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sprinting);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Disarming);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Armed);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Crouching);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SwappingShoulder);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(FireCooldown);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ShoulderSwapped);
}
