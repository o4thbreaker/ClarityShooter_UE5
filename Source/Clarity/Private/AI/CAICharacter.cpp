// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "ActionSystem/CActionComponent.h"
#include "CAttributeComponent.h"
#include "AI/CAIController.h"
#include "BrainComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Weapons/CWeaponSlotsComponent.h"
#include "Weapons/CWeaponBase.h"
#include "CGameplayTags.h"
#include "AI/CAIManager.h"

ACAICharacter::ACAICharacter()
{
	AttributeComponent = CreateDefaultSubobject<UCAttributeComponent>(TEXT("AttributeComponent"));
	ActionComponent = CreateDefaultSubobject<UCActionComponent>(TEXT("ActionComponent"));
	WeaponSlotsComponent = CreateDefaultSubobject<UCWeaponSlotsComponent>(TEXT("WeaponSlotsComponent"));

	/* ========= DEFAULT VALUES ========= */
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	PerceptionTarget = "spine_02";
	Faction = ECFaction::Enemy;
	CombatRole = ECCombatRole::Engager;
}

void ACAICharacter::Initialize()
{
	/// \NOTE: fill here anything that BT has to know (gets called from Controller)

	WeaponSlotsComponent->SpawnWeapon();
}

void ACAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ACAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnHealthChanged.AddDynamic(this, &ACAICharacter::OnHealthChanged);
}

void ACAICharacter::OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		FString OuchString = FString::Printf(TEXT("OUCH: %f"), Delta);
		DrawDebugString(GetWorld(), GetActorLocation() + FVector(0, 10, 0), OuchString, nullptr, FColor::Red, 1.5f, true);
		UE_LOG(LogTemp, Log, TEXT("%f"), Delta);

		/// \TODO: refactor to a callback to Damage Sense

		//if (InstigatorActor && InstigatorActor != this)
		//{
		//	SetTargetActor(InstigatorActor);
		//}

		if (NewHealth <= 0.0f)
		{
			// stop bt
			if (AIController)
			{
				AIController->GetBrainComponent()->StopLogic(TEXT("Killed"));
				AIController->ClearFocus(EAIFocusPriority::LastFocusPriority);
				AIController->AIManager->RemoveAgent(AIController);
				//AIController->GetAIPerceptionComponent()->DestroyComponent(true);
			}

			GetMesh()->SetCollisionProfileName("Ragdoll");
			GetMesh()->SetAllBodiesSimulatePhysics(true);

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();	

			SetLifeSpan(10.0f);
		}
	}
}

bool ACAICharacter::CanBeSeenFrom(const FVector& ObserverLocation, FHitResult& OutHitResult, const AActor* IgnoreActor) const
{
	// function that makes AI look at other bones rather than the center of the mass
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

bool ACAICharacter::GetAimOriginAndDirection(FVector& OutOrigin, FVector& OutDirection) const
{
	if (!ensure(AIController)) return false;

	AActor* TargetActor = AIController->GetTargetActor();
	if (TargetActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI has no target"));
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("Target for AI is: %s"), *GetNameSafe(TargetActor));

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


