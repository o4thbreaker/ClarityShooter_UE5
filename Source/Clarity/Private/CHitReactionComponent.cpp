// Fill out your copyright notice in the Description page of Project Settings.

#include "CHitReactionComponent.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "GameFramework/Character.h"
#include "CAttributeComponent.h"

UCHitReactionComponent::UCHitReactionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	HitReactionTimeRemaining = 0.f;
	CoreBodyName = FName("pelvis");
	ProfileName = FName("HitReaction");
}

void UCHitReactionComponent::BeginPlay()
{
	Super::BeginPlay();

	/// \TODO: might be better to initialize it in Character class, but for now i want to make this component as modular as possible 
	//         (and i dont have base class for both player and ai) so we will add physical animation component there
	PhysicalAnimationComponent = Cast<UPhysicalAnimationComponent>(GetOwner()->AddComponentByClass(UPhysicalAnimationComponent::StaticClass(), false, GetOwner()->GetTransform(), false));
	OwnerMeshComponent = Cast<ACharacter>(GetOwner())->GetMesh();

	if (!ensureAlwaysMsgf(PhysicalAnimationComponent, TEXT("PhysicalAnimationComponent is null")) || !ensureAlwaysMsgf(OwnerMeshComponent, TEXT("OwnerMeshComponent is null"))) return;

	PhysicalAnimationComponent->SetSkeletalMeshComponent(OwnerMeshComponent);
	PhysicalAnimationComponent->SetComponentTickEnabled(false);

	UCAttributeComponent* OwnerAttributes = UCAttributeComponent::GetAttributes(GetOwner());
	if (OwnerAttributes)
	{
		OwnerAttributes->OnDamage.AddDynamic(this, &UCHitReactionComponent::OnOwnerDamaged);
	}
}

void UCHitReactionComponent::PerformHitReaction(const FHitResult& Hit, const float KnockbackTime, const float KnockbackForce)
{
	SetComponentTickEnabled(true);

	// setting it to lerp back to zero in tick
	HitReactionTimeRemaining += KnockbackTime;

	OwnerMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	PhysicalAnimationComponent->ApplyPhysicalAnimationProfileBelow(CoreBodyName, ProfileName, false, false);

	// set all bodies below pelvis to simulate physics
	// do not set the include self because we dont want to make character fall as a ragdoll
	OwnerMeshComponent->SetAllBodiesBelowSimulatePhysics(CoreBodyName, true, false);

	FName HitBoneName;

	(Hit.BoneName == CoreBodyName) ? HitBoneName = FName("spine_02") : HitBoneName = Hit.BoneName;

	FVector ImpulseVector = (Hit.TraceEnd - Hit.TraceStart).GetSafeNormal() * KnockbackForce;
	OwnerMeshComponent->AddImpulse(ImpulseVector, HitBoneName, true);
	PhysicalAnimationComponent->SetComponentTickEnabled(true);
}


void UCHitReactionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector HeadLoc = OwnerMeshComponent->GetBoneLocation(FName("head"));
	DrawDebugSphere(GetWorld(), HeadLoc, 12.f, 8, FColor::Red, false, 0.f);

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("%f"), HitReactionTimeRemaining));

	HitReactionTimeRemaining -= DeltaTime;

	if (HitReactionTimeRemaining <= 0.0f)
	{
		// hit reaction is finished, time to turn off physics

		// just for safety
		HitReactionTimeRemaining = 0.0f;

		PhysicalAnimationComponent->SetComponentTickEnabled(false);
		// reset profile
		PhysicalAnimationComponent->ApplyPhysicalAnimationProfileBelow(CoreBodyName, NAME_None, false, false);
		OwnerMeshComponent->SetAllBodiesBelowSimulatePhysics(CoreBodyName, false, true);
		SetComponentTickEnabled(false);
		OwnerMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		// hit reaction is in progress
		// so set the blend weight (smooth transition, interpolation basically) to the current HitReactionTimeRemaining (if it is less then 1, cuz normalized)

		// if comment this line - things will get funny believe me
		float MinValue = FMath::Min(HitReactionTimeRemaining, 1.0f);
		OwnerMeshComponent->SetAllBodiesBelowPhysicsBlendWeight(CoreBodyName, MinValue, false, true);
	}
}

void UCHitReactionComponent::OnOwnerDamaged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewHealth, FHealthChangeInfo HealthChangeInfo)
{
	// death
	if (NewHealth <= 0.0f)
	{
		HitReactionTimeRemaining = 0.0f;

		PhysicalAnimationComponent->SetComponentTickEnabled(false);
		// reset profile
		PhysicalAnimationComponent->ApplyPhysicalAnimationProfileBelow(CoreBodyName, NAME_None, false, false);
		SetComponentTickEnabled(false);
		OwnerMeshComponent->SetCollisionProfileName("Ragdoll");
		OwnerMeshComponent->SetAllBodiesSimulatePhysics(true);
		return;
	}

	// damage reaction (if not dead)
	PerformHitReaction(HealthChangeInfo.Hit, HealthChangeInfo.KnockbackTime, HealthChangeInfo.KnockbackForce);
}
