// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CAction_DisarmEnemy.h"
#include "ActionSystem/CActionComponent.h"
#include "CGameplayTags.h"
#include "Weapons/CWeaponSlotsComponent.h"
#include "CShooterInterface.h"

UCAction_DisarmEnemy::UCAction_DisarmEnemy()
{
	Range = 500.0f;
}

bool UCAction_DisarmEnemy::CanStartAction_Implementation(AActor* Instigator)
{
	// if (TargetActionComponent->ActiveGameplayTags.HasTag(CGameplayTags::Aiming)) return true;
	return true;
}

void UCAction_DisarmEnemy::StartAction_Implementation(AActor* Instigator)
{
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	/* is crosshair translated successfully */
	bool bIsCrosshairTranslated = false;
	ICShooterInterface* Shooter = Cast<ICShooterInterface>(Instigator);

	if (Shooter)
	{
		bIsCrosshairTranslated = Shooter->GetAimOriginAndDirection(CrosshairWorldPosition, CrosshairWorldDirection);
	}

	if (bIsCrosshairTranslated)
	{
		FHitResult Hit;
		FVector Start = CrosshairWorldPosition;
		FVector End = Start + (CrosshairWorldDirection * Range);
		float CapsuleRadius = 30.0f;
		float CapsuleHalfHeight = 60.0f;
		FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Instigator);

		FCollisionObjectQueryParams ObjectParams;
		ObjectParams.AddObjectTypesToQuery(ECC_Pawn);

		GetWorld()->SweepSingleByObjectType(Hit, Start, End, FQuat::Identity, ObjectParams, CapsuleShape, Params);

		FColor Color = Hit.bBlockingHit ? FColor::Green : FColor::Red;
		DrawDebugLine(GetWorld(), Start, End, Color, false, 1.0f);
		DrawDebugSphere(GetWorld(), End, 30.0f, 32, Color, false, 1.0f);
	}
}

void UCAction_DisarmEnemy::StopAction_Implementation(AActor* Instigator)
{
}
