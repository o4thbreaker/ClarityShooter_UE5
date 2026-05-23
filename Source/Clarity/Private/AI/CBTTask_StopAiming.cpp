// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CBTTask_StopAiming.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "ActionSystem/CActionComponent.h"
#include "CGameplayTags.h"
#include "AI/CAICharacter.h"
#include "AI/CAIController.h"
#include "BehaviorTree/BlackboardComponent.h" 

EBTNodeResult::Type UCBTTask_StopAiming::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	/// \WARNING: hardcoded to ACAI, which is fine (i guess)

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

		UCActionComponent* ActionComponent = Owner->FindComponentByClass<UCActionComponent>();

		if (ActionComponent == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		MyController->SetTargetActor(nullptr);

		ActionComponent->StopActionByTag(Owner, CGameplayTags::AimAction);

		// no need to return failed if we couldn't stop the aiming. in any case it is success
		// if aiming action was stopped -> success
		// if aiming action could not be stopped -> it already have been stopped somewhere else -> success
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
