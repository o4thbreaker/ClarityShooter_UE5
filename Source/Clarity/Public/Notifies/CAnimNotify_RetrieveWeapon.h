// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_RetrieveWeapon.generated.h"

/**
 * Notify that steals weapon from Victim (identified by Disarm action) and grants to Owner (probably not that performance-cheap)
 */
UCLASS()
class CLARITY_API UCAnimNotify_RetrieveWeapon : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
