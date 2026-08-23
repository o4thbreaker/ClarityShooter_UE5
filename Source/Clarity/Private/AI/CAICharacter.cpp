// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/CAICharacter.h"
#include "AI/CAIController.h"
#include "AI/CAIManagerSubsystem.h"
#include "Perception/PawnSensingComponent.h"
#include "ActionSystem/CActionComponent.h"
#include "CAttributeComponent.h"
#include "BrainComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Weapons/CWeaponSlotsComponent.h"
#include "Weapons/CWeaponBase.h"
#include "CGameplayTags.h"
#include "CHitReactionComponent.h"

ACAICharacter::ACAICharacter()
{
	HitReactionComponent = CreateDefaultSubobject<UCHitReactionComponent>(TEXT("HitReactionComponent"));

	/* ========= DEFAULT VALUES ========= */
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	PerceptionTarget = "spine_02";
	Faction = ECFaction::Enemy;
	CombatRole = ECCombatRole::Engager;
}

void ACAICharacter::Initialize()
{
	/// \NOTE: fill here anything that BT has to know (gets called from Controller)

	WeaponSlotsComponent->SpawnDefaultWeapon();
	/// \TODO: fix this workaround
	ActionComponent->ActiveGameplayTags.AddTag(CGameplayTags::Armed);
}

void ACAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnHealthChanged.AddDynamic(this, &ACAICharacter::OnDamaged);
}

void ACAICharacter::OnDamaged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewHealth, FHealthChangeInfo HealthChangeInfo)
{
	// death
	if (NewHealth <= 0.0f)
	{
		// ragdoll is in HitReactionComponent
		// controller logic is in the controller class

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->DisableMovement();	

		SetLifeSpan(10.0f);
	}
}

bool ACAICharacter::CanBeSeenFrom(const FVector& ObserverLocation, FHitResult& OutHitResult, const AActor* IgnoreActor) const
{
	// function that makes AI look at other bones rather than to the center of the mass
	/// \NOTE: is not used for now, needs review

	static const FName AILineOfSight = FName(TEXT("PawnLineOfSight"));
	FHitResult HitResult;
	FVector SocketLocation = GetMesh()->GetSocketLocation(PerceptionTarget);
	FCollisionObjectQueryParams ObjectQueryParams = FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic));
	FCollisionQueryParams Params = FCollisionQueryParams(AILineOfSight, true, IgnoreActor);

	const bool bIsHitSocket = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, SocketLocation, ObjectQueryParams, Params);

	if (bIsHitSocket == false || IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsOwnedBy(this))
	{
		OutHitResult = HitResult;

		return true;
	}

	// do the same but to ActorLocation()
	const bool bIsHit = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, GetActorLocation(), ObjectQueryParams, Params);

	if (bIsHit == false || IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsOwnedBy(this))
	{
		OutHitResult = HitResult;

		return true;
	}

	return false;
}

void ACAICharacter::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	if (GetMesh()->DoesSocketExist(TEXT("EyesSocket")))
	{
		OutLocation = GetMesh()->GetSocketLocation(TEXT("EyesSocket"));
		OutRotation = GetMesh()->GetSocketRotation(TEXT("EyesSocket"));
	}
	else
	{
		OutLocation = GetActorLocation() + FVector(0, 0, BaseEyeHeight);
		OutRotation = GetActorRotation();
	}
}

bool ACAICharacter::IsHostile(AActor* Other)
{
	if (ICShooterInterface* OtherShooterInterface = Cast<ICShooterInterface>(Other))
	{
		return Faction != OtherShooterInterface->GetFaction() && OtherShooterInterface->GetFaction() != ECFaction::Neutral;
	}

	return false;
}

void ACAICharacter::SetIsCombat(bool bNewIsCombat)
{
	AnimState.bIsCombat = bNewIsCombat;
	if (bNewIsCombat)
	{
		ActionComponent->StartActionByTag(this, CGameplayTags::AimAction);
	}
	else
	{
		/// \NOTE: probably do something more interesting in future like draw a weapon not just aim it
		ActionComponent->StopActionByTag(this, CGameplayTags::AimAction);
	}
}

void ACAICharacter::SetIsCrouching(bool bNewIsCrouching)
{
	AnimState.bIsCrouching = bNewIsCrouching;
	if (bNewIsCrouching)
	{
		ActionComponent->ActiveGameplayTags.AddTag(CGameplayTags::Crouching);
	}
	else
	{
		ActionComponent->ActiveGameplayTags.RemoveTag(CGameplayTags::Crouching);
	}
}

bool ACAICharacter::GetAimOriginAndDirection(FVector& OutOrigin, FVector& OutDirection) const
{
	if (!ensure(AIController)) return false;

	AActor* TargetActor = AIController->GetTargetActor();
	if (TargetActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI has no target"));
		return false;
	}

	ACWeaponBase* Weapon = WeaponSlotsComponent->GetCurrentWeapon();
	if (Weapon == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI has no weapon"));
		return false;
	}

	OutOrigin = Weapon->GetMesh()->GetSocketTransform(Weapon->BarrelSocketName).GetLocation();

	/// \TODO: maybe create a better aiming system for AI
	OutDirection = (TargetActor->GetActorLocation() - OutOrigin).GetSafeNormal();

	return true;
}

bool ACAICharacter::GetIsAiming() const
{
	return ActionComponent->ActiveGameplayTags.HasTag(CGameplayTags::Aiming);
}


