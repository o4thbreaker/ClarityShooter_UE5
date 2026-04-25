// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void UCBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	if (ensure(BlackboardComponent))
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActor"));

		if (TargetActor)
		{
			AAIController* MyController = OwnerComp.GetAIOwner();

			if (ensure(MyController))
			{
				APawn* AIPawn = MyController->GetPawn();

				if (ensure(AIPawn))
				{
					float DistanceTo = FVector::Distance(AIPawn->GetActorLocation(), TargetActor->GetActorLocation());

					bool IsInRange = DistanceTo <= AttackRange;
					bool HasLineOfSight = false;

					if (IsInRange)
					{
						HasLineOfSight = MyController->LineOfSightTo(TargetActor);
					}

					BlackboardComponent->SetValueAsBool(InAttackRangeKey.SelectedKeyName, (IsInRange && HasLineOfSight));
				}
			}
		}
	}

}
