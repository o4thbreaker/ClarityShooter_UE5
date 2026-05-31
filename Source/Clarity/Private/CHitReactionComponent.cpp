// Fill out your copyright notice in the Description page of Project Settings.

#include "CHitReactionComponent.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "GameFramework/Character.h"

UCHitReactionComponent::UCHitReactionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	HitReactionTimeRemaining = 0.f;
	CoreBodyName = FName("pelvis");
	ProfileName = FName("HitReaction");
	KnockbackTime = 1.0f;
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

	UE_LOG(LogTemp, Warning, TEXT("PAC Tick: BoneSpaceTransforms.Num() = %d, TimeRemaining = %f"),
		OwnerMeshComponent->GetBoneSpaceTransforms().Num(),
		HitReactionTimeRemaining);

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("%f"), HitReactionTimeRemaining));

	HitReactionTimeRemaining -= DeltaTime;

	if (HitReactionTimeRemaining <= 0.0f)
	{
		// hit reaction is finished, time to turn off physics

		// just in case
		HitReactionTimeRemaining = 0.0f;

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

void UCHitReactionComponent::HitReaction(FHitResult Hit)
{
	SetComponentTickEnabled(true);

	// setting it to lerp back to zero in tick
	HitReactionTimeRemaining += KnockbackTime;

	PendingHit = Hit;
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UCHitReactionComponent::ApplyHitPhysics);
}

void UCHitReactionComponent::ApplyHitPhysics()
{
	OwnerMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	PhysicalAnimationComponent->ApplyPhysicalAnimationProfileBelow(CoreBodyName, ProfileName, false, false);

	/// \FIXME: isn't that will cause every body below pelvis to simulate physics and not the one that was actually hit?
	// do not set the include self because we dont want to make character fall as a ragdoll
	OwnerMeshComponent->SetAllBodiesBelowSimulatePhysics(CoreBodyName, true, false);

	FName HitBoneName;

	/// \TODO: maybe should be not pelvis but CoreBodyName but I have to make sure that FIXME is wrong
	PendingHit.BoneName == FName("pelvis") ? HitBoneName = FName("spine_02") : HitBoneName = PendingHit.BoneName;

	/// \TODO: fix the magic number
	FVector ImpulseVector = (PendingHit.TraceEnd - PendingHit.TraceStart).GetSafeNormal() * 5000.f;
	OwnerMeshComponent->AddImpulse(ImpulseVector, HitBoneName, true);
}



