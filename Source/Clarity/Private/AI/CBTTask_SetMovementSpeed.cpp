// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CBTTask_SetMovementSpeed.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "AIController.h"

UCBTTask_SetMovementSpeed::UCBTTask_SetMovementSpeed()
{
	WalkingSpeed = 100.0f;
	JoggingSpeed = 300.0f;
	SprintingSpeed = 500.0f;

	DesiredMovementMode = ECMovementMode::Idle;
}

EBTNodeResult::Type UCBTTask_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = Cast<AAIController>(OwnerComp.GetAIOwner());
	ACharacter* Owner = Cast<ACharacter>(MyController->GetPawn());
	
	if (ensure(Owner))
	{
		UCharacterMovementComponent* OwnerMovementComponent = Owner->GetCharacterMovement();
		switch (DesiredMovementMode)
		{
		case ECMovementMode::Idle:
			OwnerMovementComponent->MaxWalkSpeed = 0.0f;
			return EBTNodeResult::Succeeded;
			break;
		case ECMovementMode::Walking:
			OwnerMovementComponent->MaxWalkSpeed = WalkingSpeed;
			return EBTNodeResult::Succeeded;
			break;
		case ECMovementMode::Jogging:
			OwnerMovementComponent->MaxWalkSpeed = JoggingSpeed;
			return EBTNodeResult::Succeeded;
			break;
		case ECMovementMode::Sprinting:
			OwnerMovementComponent->MaxWalkSpeed = SprintingSpeed;
			return EBTNodeResult::Succeeded;
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("Something clearly went wrong in SetMovementSpeed task"));
			return EBTNodeResult::Failed;
			break;
		}
	}

	return EBTNodeResult::Failed;
}
