// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CBTService_CheckAmmoLevel.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Weapons/CWeaponSlotsComponent.h"
#include "Weapons/CWeaponBase.h"

void UCBTService_CheckAmmoLevel::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{	
		APawn* AIPawn = MyController->GetPawn();
		if (ensure(AIPawn))
		{
			UCWeaponSlotsComponent* WeaponSlotsComponent = AIPawn->FindComponentByClass<UCWeaponSlotsComponent>();
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

			if (ensureMsgf(Weapon->GetWeaponData(), TEXT("WeaponData for %s, that is used by %s is not provided. Please provide it in Weapon's Blueprint"), *GetNameSafe(Weapon), *GetNameSafe(AIPawn)))
			{
				float CurrentAmmoPercentage = (Weapon->GetCurrentAmmoCount() / (float)Weapon->GetWeaponData()->MagazineSize) * 100.0f;

				BlackboardComponent->SetValueAsBool(LowAmmoKey.SelectedKeyName, CurrentAmmoPercentage <= LowAmmoPercentage);
			}
		}
	}
}