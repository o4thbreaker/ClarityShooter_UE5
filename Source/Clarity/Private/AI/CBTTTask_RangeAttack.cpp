// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CBTTTask_RangeAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "ActionSystem/CActionComponent.h"
#include "CGameplayTags.h"
#include "ActionSystem/CAction_Shoot.h"
#include "BehaviorTree/BlackboardComponent.h" 
#include "AI/CAICharacter.h"


EBTNodeResult::Type UCBTTTask_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

		Owner->SetCurrentTarget(Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor")));
		
		return ActionComponent->StartActionByTag(Owner, CGameplayTags::FireAction) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}
