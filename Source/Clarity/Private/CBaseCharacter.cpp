// Fill out your copyright notice in the Description page of Project Settings.


#include "CBaseCharacter.h"
#include "CBaseAnimInstance.h"
#include "Weapons/CWeaponSlotsComponent.h"
#include "ActionSystem/CActionComponent.h"
#include "CAttributeComponent.h"
#include "CGameplayTags.h"
#include "MotionWarpingComponent.h"
#include "ContextualAnimSceneActorComponent.h"

ACBaseCharacter::ACBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<UCAttributeComponent>(TEXT("AttributeComponent"));
	ActionComponent = CreateDefaultSubobject<UCActionComponent>(TEXT("ActionComponent"));
	WeaponSlotsComponent = CreateDefaultSubobject<UCWeaponSlotsComponent>(TEXT("WeaponSlotsComponent"));
	ContextualAnimComponent = CreateDefaultSubobject<UContextualAnimSceneActorComponent>(TEXT("ContextualAnimComponent"));
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));

	bIsInContextualAnimMode = false;
}

bool ACBaseCharacter::GetAimOriginAndDirection(FVector& OutOrigin, FVector& OutDirection) const
{
	// will be implemented in derived classes
	return false;
}

ECFaction ACBaseCharacter::GetFaction() const
{
	// will be implemented in derived classes
	return ECFaction();
}

void ACBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	WeaponSlotsComponent->OnWeaponEquiped.AddDynamic(this, &ACBaseCharacter::OnWeaponEquiped);
	WeaponSlotsComponent->OnWeaponLost.AddDynamic(this, &ACBaseCharacter::OnWeaponLost);

	// due to incompatible order we have tot make it idempotent-readable
	if (WeaponSlotsComponent->GetCurrentWeapon())
	{
		OnWeaponEquiped(WeaponSlotsComponent, WeaponSlotsComponent->GetCurrentWeapon());
	}

	BaseAnimInstance = Cast<UCBaseAnimInstance>(GetMesh()->GetAnimInstance());

	ContextualAnimComponent->OnJoinedSceneDelegate.AddDynamic(this, &ACBaseCharacter::OnContextualAnimStateChange);
	ContextualAnimComponent->OnLeftSceneDelegate.AddDynamic(this, &ACBaseCharacter::OnContextualAnimStateChange);
}

void ACBaseCharacter::OnWeaponEquiped(UCWeaponSlotsComponent* OwningComp, ACWeaponBase* Weapon)
{
	ActionComponent->ActiveGameplayTags.AddTag(CGameplayTags::Armed);
}

void ACBaseCharacter::OnWeaponLost(UCWeaponSlotsComponent* OwningComp)
{
	ActionComponent->ActiveGameplayTags.RemoveTag(CGameplayTags::Armed);
}

void ACBaseCharacter::OnContextualAnimStateChange(UContextualAnimSceneActorComponent* SceneActorComponent)
{
	bIsInContextualAnimMode ? bIsInContextualAnimMode = false : bIsInContextualAnimMode = true;	
}
