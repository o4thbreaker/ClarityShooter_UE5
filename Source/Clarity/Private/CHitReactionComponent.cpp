// Fill out your copyright notice in the Description page of Project Settings.

#include "CHitReactionComponent.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

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
}

void UCHitReactionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// \FIXME: isn't it like insta 0?
	HitReactionTimeRemaining = UKismetMathLibrary::FInterpTo(HitReactionTimeRemaining, 0.0f, 0.0f, 1.0f) - (DeltaTime * 0.1f);

	if (HitReactionTimeRemaining <= 0.0f)
	{
		// just in case
		HitReactionTimeRemaining = 0.0f;

		OwnerMeshComponent->SetAllBodiesBelowSimulatePhysics(CoreBodyName, false, true);
		SetComponentTickEnabled(false);
		OwnerMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		/// \TODO: dig dipper to understand this part of code
		float MinValue = UKismetMathLibrary::Min(HitReactionTimeRemaining, 1.0f);
		OwnerMeshComponent->SetAllBodiesBelowPhysicsBlendWeight(CoreBodyName, MinValue, false, true);
	}
}

void UCHitReactionComponent::HitReaction(FHitResult Hit)
{
	SetComponentTickEnabled(true);

	/// \TODO: fix the magic number.. and make sure what this is
	HitReactionTimeRemaining += 2.0f;

	OwnerMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	PhysicalAnimationComponent->ApplyPhysicalAnimationProfileBelow(CoreBodyName, ProfileName, false, false);

	/// \FIXME: isn't that will cause every body to simulate physics and not the one that was actually hit?
	// do not set the include self because we dont want to make character fall as a ragdoll
	OwnerMeshComponent->SetAllBodiesBelowSimulatePhysics(CoreBodyName, true, false);

	FName HitBoneName;

	/// \TODO: maybe should be not pelvis but CoreBodyName but I have to make sure that FIXME is wrong
	Hit.BoneName == FName("pelvis") ? HitBoneName = FName("spine_02") : HitBoneName = Hit.BoneName;

	if (OwnerMeshComponent->GetCollisionEnabled() == ECollisionEnabled::QueryAndPhysics)
	{
		FVector ImpulseVector = (Hit.TraceEnd - Hit.TraceStart).GetSafeNormal() * 5000.f;
		OwnerMeshComponent->AddImpulse(ImpulseVector, HitBoneName, true);
	}
}



