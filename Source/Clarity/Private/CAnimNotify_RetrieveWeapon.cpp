// Fill out your copyright notice in the Description page of Project Settings.


#include "CAnimNotify_RetrieveWeapon.h"
#include "CBaseCharacter.h"
#include "ContextualAnimSceneActorComponent.h"
#include "Weapons/CWeaponSlotsComponent.h"

void UCAnimNotify_RetrieveWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	UE_LOG(LogTemp, Log, TEXT("Retrieve weapon notify!"));
	ACBaseCharacter* BaseCharacter = Cast<ACBaseCharacter>(MeshComp->GetOwner());

	if (BaseCharacter)
	{
		UContextualAnimSceneActorComponent* ContextualAnimComponent = BaseCharacter->GetContextualAnimComponent();
		if (ContextualAnimComponent)
		{
			/// \NOTE: finding by name tstkstskstsk
			AActor* VictimActor = ContextualAnimComponent->GetBindings().FindBindingByRole(FName("Victim"))->GetActor();
			if (VictimActor)
			{
				UE_LOG(LogTemp, Log, TEXT("Victim is %s"), *GetNameSafe(VictimActor));

				UCWeaponSlotsComponent* TargetWeaponSlotsComponent = UCWeaponSlotsComponent::GetWeaponSlotsComponent(VictimActor);
				if (!TargetWeaponSlotsComponent) return;

				UCWeaponSlotsComponent* OwnerWeaponSlotsComponent = UCWeaponSlotsComponent::GetWeaponSlotsComponent(BaseCharacter);
				if (!OwnerWeaponSlotsComponent) return;

				ACWeaponBase* TargetWeapon = TargetWeaponSlotsComponent->LoseCurrentWeapon();
				OwnerWeaponSlotsComponent->SwitchWeapon(TargetWeapon);
			}
		}
	}

}
