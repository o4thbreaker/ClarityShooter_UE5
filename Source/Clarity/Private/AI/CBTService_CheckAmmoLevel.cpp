// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CBTService_CheckAmmoLevel.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Weapons/CWeaponSlotsComponent.h"
#include "Weapons/CWeaponBase.h"
#include "AI/CBTAmmoObserver.h"

UCBTService_CheckAmmoLevel::UCBTService_CheckAmmoLevel()
{
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
	bNotifyTick = false;
}

void UCBTService_CheckAmmoLevel::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Log, TEXT("UCBTService_CheckAmmoLevel::OnBecomeRelevant called"));
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	FCheckAmmoLevelMemory* Memory = CastInstanceNodeMemory<FCheckAmmoLevelMemory>(NodeMemory);
	if (!ensure(Memory)) return;

	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		// we instantiate the observer to controller because observer should die only when controller dies
		Memory->Observer = NewObject<UCBTAmmoObserver>(MyController);

		// fill the observer's data: blackboard component, weapon reference and low ammo percentage and key selector
		Memory->Observer->BlackboardComponent = OwnerComp.GetBlackboardComponent();
		if (Memory->Observer->BlackboardComponent == nullptr) return;
	
		APawn* AIPawn = MyController->GetPawn();
		if (ensure(AIPawn))
		{
			UCWeaponSlotsComponent* WeaponSlotsComponent = AIPawn->FindComponentByClass<UCWeaponSlotsComponent>();
			if (WeaponSlotsComponent == nullptr) return;
			
			Memory->Observer->Weapon = WeaponSlotsComponent->GetCurrentWeapon();
			if (Memory->Observer->Weapon == nullptr) return;

			Memory->Observer->LowAmmoPercentage = LowAmmoPercentage;
			Memory->Observer->LowAmmoKeyName = LowAmmoKey.SelectedKeyName;
			
			Memory->Observer->OnAmmoChanged(Memory->Observer->Weapon->GetCurrentAmmoCount());
			Memory->Observer->Weapon->OnAmmoChanged.AddDynamic(Memory->Observer, &UCBTAmmoObserver::OnAmmoChanged);
		}
	}
}

void UCBTService_CheckAmmoLevel::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Log, TEXT("UCBTService_CheckAmmoLevel::OnCeaseRelevant called"));
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);

	FCheckAmmoLevelMemory* Memory = CastInstanceNodeMemory<FCheckAmmoLevelMemory>(NodeMemory);

	if (IsValid(Memory->Observer))
	{
		if (IsValid(Memory->Observer->Weapon))
		{
			Memory->Observer->Weapon->OnAmmoChanged.RemoveDynamic(Memory->Observer, &UCBTAmmoObserver::OnAmmoChanged);
		}
	}
}

void UCBTService_CheckAmmoLevel::AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector)
{
	/// \NOTE: this is needed to make sure that the observer we created gets garbage collected properly
	///        Since it's not a UPROPERTY, we need to add it here
	UCBTService_CheckAmmoLevel* This = CastChecked<UCBTService_CheckAmmoLevel>(InThis);

	Super::AddReferencedObjects(InThis, Collector);
}

uint16 UCBTService_CheckAmmoLevel::GetInstanceMemorySize() const
{
	return sizeof(FCheckAmmoLevelMemory);
}