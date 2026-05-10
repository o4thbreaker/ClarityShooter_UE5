// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CBTTask_RangeAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "ActionSystem/CActionComponent.h"
#include "CGameplayTags.h"
#include "ActionSystem/CAction_Shoot.h"
#include "BehaviorTree/BlackboardComponent.h" 
#include "AI/CAICharacter.h"
#include "AI/CAIController.h"
#include "Weapons/CWeaponSlotsComponent.h"
#include "Weapons/CWeaponBase.h"

UCBTTask_RangeAttack::UCBTTask_RangeAttack()
{
	MagazineThresholdPercentage = 10.0f;

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTask_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* MyController = Cast<ACAIController>(OwnerComp.GetAIOwner());

	if (ensure(MyController))
	{
		FRangeAttackDataMemory* Memory = CastInstanceNodeMemory<FRangeAttackDataMemory>(NodeMemory);

		/// \WARNING: hardcoded to ACAICharacter
		Memory->Owner = Cast<ACAICharacter>(MyController->GetPawn());

		if (Memory->Owner == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		
		/// \TODO
		/*if (!UArtAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}*/

		Memory->OwnerActionComponent = Memory->Owner->FindComponentByClass<UCActionComponent>();

		if (Memory->OwnerActionComponent == nullptr )
		{
			return EBTNodeResult::Failed;
		}

		Memory->OwnerWeaponSlotsComponent = Memory->Owner->FindComponentByClass<UCWeaponSlotsComponent>();
		if (Memory->OwnerWeaponSlotsComponent == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		
		/// \WARNING: we assume that the target is already set by SetAiming task 
		
		if (!MyController->GetCurrentTarget())
		{
			UE_LOG(LogTemp, Warning, TEXT("No target set. Aborting task."));
			return EBTNodeResult::Failed;
		}

		Memory->OwnerActionComponent->StartActionByTag(Memory->Owner, CGameplayTags::FireAction);

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UCBTTask_RangeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FRangeAttackDataMemory* Memory = CastInstanceNodeMemory<FRangeAttackDataMemory>(NodeMemory);

	if (!IsMemoryValid(OwnerComp, NodeMemory, Memory)) return;

	if (!Memory->OwnerActionComponent->ActiveGameplayTags.HasTag(CGameplayTags::FireCooldown))
	{
		// weapon is not on a cooldown
		ACWeaponBase* Weapon = Memory->OwnerWeaponSlotsComponent->GetCurrentWeapon();
		if (!Weapon)
		{
			// no weapon attached
			UE_LOG(LogTemp, Warning, TEXT("No Weapon attached. Aborting task."));
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		// calculate the remaigning percentage of the gun
		float CurrentPercentage = (Weapon->GetCurrentAmmoCount() / (float)Weapon->GetWeaponData()->MagazineSize) * 100.0f;
		if (CurrentPercentage > MagazineThresholdPercentage)
		{
			// we still can shoot

			if (!Memory->OwnerActionComponent->StartActionByTag(Memory->Owner, CGameplayTags::FireAction))
			{
				// for some reason we couldn't start the action, so we had to finish earlier than planned
				UE_LOG(LogTemp, Warning, TEXT("Couldn't start Shoot for %s. Aborting task."), *OwnerComp.GetOwner()->GetName());
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			}
		}
		else
		{
			// we have fewer ammo than allowed, succesfully retreat
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
		
}

uint16 UCBTTask_RangeAttack::GetInstanceMemorySize() const
{
	return sizeof(FRangeAttackDataMemory);
}

bool UCBTTask_RangeAttack::IsMemoryValid(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FRangeAttackDataMemory* Memory)
{
	if (!IsValid(Memory->Owner))
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner is nullptr or pending to kill. Aborting task."));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return false;
	}

	if (!IsValid(Memory->OwnerActionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("No ActionComponent attached. Aborting task."));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return false;
	}

	if (!IsValid(Memory->OwnerWeaponSlotsComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("No WeaponSlotsComponent attached. Aborting task."));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return false;
	}

	return true;
}
