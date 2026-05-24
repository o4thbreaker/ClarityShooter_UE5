// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CBTTask_Reload.h"
#include "GameFramework/Pawn.h"
#include "AI/CAIController.h"
#include "AI/CAICharacter.h"
#include "ActionSystem/CActionComponent.h"
#include "ActionSystem/CAction.h"
#include "CGameplayTags.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/CAIManager.h"

UCBTTask_Reload::UCBTTask_Reload()
{
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UCBTTask_Reload::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* MyController = Cast<ACAIController>(OwnerComp.GetAIOwner());

	if (ensure(MyController))
	{
		FReloadDataMemory* Memory = CastInstanceNodeMemory<FReloadDataMemory>(NodeMemory);

		if (MyController->GetAICharacter() == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Couldn't get Character on AI. Aborting task!"));
			return EBTNodeResult::Failed;
		}

		Memory->OwnerActionComponent = MyController->GetAICharacter()->GetActionComponent();
		if (Memory->OwnerActionComponent == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Couldn't find ActionComponent on AI. Aborting task!"));
			return EBTNodeResult::Failed;
		}

		if (!Memory->OwnerActionComponent->StartActionByTag(MyController->GetAICharacter(), CGameplayTags::ReloadAction))
		{
			return EBTNodeResult::Failed;
		}

		// tell others to cover while reloading
		MyController->SetShouldShootFromCover(false);
		MyController->AIManager->RequestCoverFire(true, MyController->GetAICharacter());

		Memory->OwnerActionComponent->OnActionStopped.AddDynamic(this, &UCBTTask_Reload::OnReloadFinished);

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}


void UCBTTask_Reload::OnReloadFinished(UCActionComponent* ActionComponent, UCAction* Action)
{
	if (Action->ActionTag == CGameplayTags::ReloadAction)
	{
		APawn* OwnerPawn = Cast<APawn>(ActionComponent->GetOwner());
		AAIController* AIController = Cast<AAIController>(OwnerPawn->GetController());
		if (ensureAlways(AIController))
		{
			UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(AIController->BrainComponent);
			if (ensure(BehaviorTreeComponent))
			{
				FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Succeeded);
			}
		}
	}
}

void UCBTTask_Reload::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	FReloadDataMemory* Memory = CastInstanceNodeMemory<FReloadDataMemory>(NodeMemory);
	if (ensure(Memory->OwnerActionComponent))
	{
		Memory->OwnerActionComponent->OnActionStopped.RemoveDynamic(this, &UCBTTask_Reload::OnReloadFinished);	
	}

	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

uint16 UCBTTask_Reload::GetInstanceMemorySize() const
{
	return sizeof(FReloadDataMemory);
}

