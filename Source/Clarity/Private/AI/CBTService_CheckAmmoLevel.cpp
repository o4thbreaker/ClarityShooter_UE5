// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CBTService_CheckAmmoLevel.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/CAIController.h"
#include "AI/CAICharacter.h"
#include "Weapons/CWeaponSlotsComponent.h"
#include "Weapons/CWeaponBase.h"

void UCBTService_CheckAmmoLevel::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* AIController = Cast<ACAIController>(OwnerComp.GetAIOwner());
	if (ensure(AIController))
	{	
		ACAICharacter* AICharacter = AIController->GetAICharacter();
		if (ensure(AICharacter))
		{
			UCWeaponSlotsComponent* WeaponSlotsComponent = AICharacter->GetWeaponSlotsComponent();
			if (WeaponSlotsComponent == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Couldn't find WeaponSlotsComponent on AI. Aborting service!"));
				return;
			}

			ACWeaponBase* Weapon = WeaponSlotsComponent->GetCurrentWeapon();
			if (Weapon == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Couldn't find current weapon on AI. Aborting service!"));
				return;
			}

			if (ensureMsgf(Weapon->GetWeaponData(), TEXT("WeaponData for %s, that is used by %s is not provided. Please provide it in Weapon's Blueprint"), *GetNameSafe(Weapon), *GetNameSafe(AICharacter)))
			{
				float CurrentAmmoPercentage = (Weapon->GetCurrentAmmoCount() / (float)Weapon->GetWeaponData()->MagazineSize) * 100.0f;
				uint8 CurrentCombatState = BlackboardComponent->GetValueAsEnum(TEXT("CombatState"));

				if (CurrentAmmoPercentage <= LowAmmoPercentage && CurrentCombatState != (uint8)ECCombatState::NeedToReload)
				{
					BlackboardComponent->SetValueAsEnum(TEXT("CombatState"), (uint8)ECCombatState::NeedToReload);
				}
			}
		}
	}
}