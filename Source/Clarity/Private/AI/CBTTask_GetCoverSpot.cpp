// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CBTTask_GetCoverSpot.h"
#include "CCoverActor.h"
#include "AI/CAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"

UCBTTask_GetCoverSpot::UCBTTask_GetCoverSpot()
{
	DesiredAngleToTarget = 45.f;

	/// \TODO: switch to NodeMemory
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UCBTTask_GetCoverSpot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{ 
	ACAIController* MyController = Cast<ACAIController>(OwnerComp.GetAIOwner());

	ACCoverActor* Cover = Cast<ACCoverActor>(MyController->GetBlackboardComponent()->GetValueAsObject("CoverActor"));
	AActor* Target = MyController->GetTargetActor();

	if (MyController && Cover && Cover->CoverSpots.Num() > 0 && Target)
	{
		MyController->GetBlackboardComponent()->SetValueAsVector("MoveToLocation", FVector::ZeroVector);
		FVector Destination = FVector::ZeroVector;
		float AngleToTarget = 0.f;

		for (auto& Arrow : Cover->CoverSpots)
		{
			const FVector ArrowForwardVector = UKismetMathLibrary::GetForwardVector(Arrow->GetComponentRotation());
			/// \TODO: check for improvements
			const FVector ArrowToTargetVector = UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::FindLookAtRotation(Arrow->GetComponentLocation(), Target->GetActorLocation()));
			const double DotProduct = UKismetMathLibrary::Dot_VectorVector(ArrowForwardVector, ArrowToTargetVector);
			AngleToTarget = UKismetMathLibrary::DegAcos(DotProduct);

			if (AngleToTarget <= DesiredAngleToTarget)
			{
				Destination = Arrow->GetComponentLocation();
				break;
			}
		}

		// if calculated angel lies between 45 degrees in front of the arrow
		if (Destination != FVector::ZeroVector)
		{
			MyController->GetBlackboardComponent()->SetValueAsVector("MoveToLocation", Destination);
		}
	}

	return EBTNodeResult::Succeeded;
}
