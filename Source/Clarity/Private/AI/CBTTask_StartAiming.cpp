// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CBTTask_StartAiming.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "ActionSystem/CActionComponent.h"
#include "CGameplayTags.h"
#include "AI/CAICharacter.h"
#include "AI/CAIController.h"
#include "BehaviorTree/BlackboardComponent.h" 

EBTNodeResult::Type UCBTTask_StartAiming::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	/// \WARNING: hardcoded to ACAI, which if fine (i guess)

	ACAIController* MyController = Cast<ACAIController>(OwnerComp.GetAIOwner());
	ACAICharacter* Owner = Cast<ACAICharacter>(MyController->GetPawn());

	if (ensure(MyController))
	{
		if (Owner == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		/// \TODO
		/*if (!UArtAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}*/

		if (Owner->GetIsAiming())
		{
			// if we're already aiming that's fine
			return EBTNodeResult::Succeeded;
		}

		UCActionComponent* ActionComponent = Owner->FindComponentByClass<UCActionComponent>();

		if (ActionComponent == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		MyController->SetTargetActor(Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName)));
		Owner->SetIsAiming(true);

		return ActionComponent->StartActionByTag(Owner, CGameplayTags::AimAction) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}
