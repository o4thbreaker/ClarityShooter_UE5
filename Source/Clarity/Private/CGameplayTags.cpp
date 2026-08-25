// Fill out your copyright notice in the Description page of Project Settings.


#include "CGameplayTags.h"

namespace CGameplayTags
{
	// ========== ACTIONS ==========
	UE_DEFINE_GAMEPLAY_TAG(ReloadAction, "Action.Reload");
	UE_DEFINE_GAMEPLAY_TAG(FireAction, "Action.Fire");
	UE_DEFINE_GAMEPLAY_TAG(AimAction, "Action.Aim");
	UE_DEFINE_GAMEPLAY_TAG(SprintAction, "Action.Sprint");
	UE_DEFINE_GAMEPLAY_TAG(AimCameraAction, "Action.AimCamera");
	UE_DEFINE_GAMEPLAY_TAG(SprintCameraAction, "Action.SprintCamera");
	UE_DEFINE_GAMEPLAY_TAG(DisarmAction, "Action.Disarm");
	UE_DEFINE_GAMEPLAY_TAG(SwapShoulderAction, "Action.SwapShoulder");
	UE_DEFINE_GAMEPLAY_TAG(HitReactionAction, "Action.HitReaction");
	UE_DEFINE_GAMEPLAY_TAG(DeathAction, "Action.Death");

	// ========== STATES ==========
	UE_DEFINE_GAMEPLAY_TAG(Reloading, "State.Reloading");
	UE_DEFINE_GAMEPLAY_TAG(Firing, "State.Firing");
	UE_DEFINE_GAMEPLAY_TAG(Aiming, "State.Aiming");
	UE_DEFINE_GAMEPLAY_TAG(Sprinting, "State.Sprinting");
	UE_DEFINE_GAMEPLAY_TAG(Disarming, "State.Disarming");
	UE_DEFINE_GAMEPLAY_TAG(Armed, "State.Armed");
	UE_DEFINE_GAMEPLAY_TAG(Crouching, "State.Crouching");
	UE_DEFINE_GAMEPLAY_TAG(SwappingShoulder, "State.SwappingShoulder");

	// ========== CONDITIONS ==========
	UE_DEFINE_GAMEPLAY_TAG(FireCooldown, "Condition.FireCooldown");
	UE_DEFINE_GAMEPLAY_TAG(ShoulderSwapped, "Condition.ShoulderSwapped");
}
