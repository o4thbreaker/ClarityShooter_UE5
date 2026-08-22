// Fill out your copyright notice in the Description page of Project Settings.


#include "CBaseAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"

void UCBaseAnimInstance::NativeInitializeAnimation()
{
	TargetRecoil = FTransform::Identity;
	ActualRecoil = FTransform::Identity;
	RecoilRecoverySpeed = 20.0f;
}

void UCBaseAnimInstance::DoProceduralRecoil(float RecoilMultiplier)
{
	/// \FIXME: recoil is always leaning to the right (not even up).
	///		 // isn't big of a deal, but probably needs to be fixed later. 

	/* ROTATION (RECOIL ITSELF) */

	float RandomRotationRoll = FMath::RandRange(-6.f, -4.f) * RecoilMultiplier; // up (negative) or down (positive)
	float RandomRotationPitch = FMath::RandRange(-2.5f, 2.5f) * RecoilMultiplier; // little roll to the left or right
	float RandomRotationYaw = 0.f * RecoilMultiplier; // left (negative) or right (positive)

	FRotator RecoilRotation = FRotator(RandomRotationPitch, RandomRotationYaw, RandomRotationRoll);

	/* LOCATION (OF THE WEAPON) */
	float RandomLocationX = FMath::RandRange(-0.1f, 0.1f) * RecoilMultiplier; // forward/backward
	float RandomLocationY = FMath::RandRange(-2.f, -2.5f) * RecoilMultiplier; // left/right
	float RandomLocationZ = 0.0f; // up/down

	FVector RecoilLocation = FVector(RandomLocationX, RandomLocationY, RandomLocationZ);

	TargetRecoil = FTransform(RecoilRotation, RecoilLocation, FVector(1, 1, 1));
}


void UCBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ActualRecoil = UKismetMathLibrary::TInterpTo(ActualRecoil, TargetRecoil, DeltaSeconds, RecoilRecoverySpeed);

	if (!TargetRecoil.Equals(FTransform::Identity))
	{
		// smoothly return to no recoil when we're not shooting
		/// \TODO: fix the magic number
		TargetRecoil = UKismetMathLibrary::TInterpTo(TargetRecoil, FTransform::Identity, DeltaSeconds, 15.0f);
	}
}