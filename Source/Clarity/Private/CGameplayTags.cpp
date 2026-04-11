// Fill out your copyright notice in the Description page of Project Settings.


#include "CGameplayTags.h"

namespace CGameplayTags
{
	// ========== ACTIONS ==========
	UE_DEFINE_GAMEPLAY_TAG(ReloadAction, "Action.Reload");
	UE_DEFINE_GAMEPLAY_TAG(FireAction, "Action.Fire");
	UE_DEFINE_GAMEPLAY_TAG(AimAction, "Action.Aim");
	UE_DEFINE_GAMEPLAY_TAG(SprintAction, "Action.Sprint");

	// ========== STATES ==========
	UE_DEFINE_GAMEPLAY_TAG(Reloading, "State.Reloading");
	UE_DEFINE_GAMEPLAY_TAG(Firing, "State.Firing");
	UE_DEFINE_GAMEPLAY_TAG(Aiming, "State.Aiming");
	UE_DEFINE_GAMEPLAY_TAG(Sprinting, "State.Sprinting");

	// ========== CONDITIONS ==========
	UE_DEFINE_GAMEPLAY_TAG(FireCooldown, "Condition.FireCooldown");
}
