// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CBTTask_StopAiming.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "ActionSystem/CActionComponent.h"
#include "CGameplayTags.h"
#include "AI/CAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h" 

EBTNodeResult::Type UCBTTask_StopAiming::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();

	/// \WARNING: hardcoded to ACAICharacter
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

		UCActionComponent* ActionComponent = Owner->FindComponentByClass<UCActionComponent>();

		if (ActionComponent == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		Owner->SetCurrentTarget(nullptr);
		Owner->SetIsAiming(false);

		return ActionComponent->StopActionByTag(Owner, CGameplayTags::AimAction) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}
