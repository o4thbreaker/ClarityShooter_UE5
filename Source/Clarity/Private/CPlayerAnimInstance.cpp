// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlayerAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"

void UCPlayerAnimInstance::NativeInitializeAnimation()
{
	TargetRecoil = FTransform::Identity;
	ActualRecoil = FTransform::Identity;
	RecoilRecoverySpeed = 20.0f;
}

void UCPlayerAnimInstance::DoProceduralRecoil(float RecoilMultiplier)
{
	/// \FIXME: recoil is always leaning to the right (not even up).
	///		 // isn't big of a deal, but probably needs to be fixed later. 

	/* ROTATION (RECOIL ITSELF) */
	/// \NOTE: i think it is pseudo random (they don't change each shot)
	float RandomRotationRoll = FMath::RandRange(-3.f, 3.f) * RecoilMultiplier; // little roll to the left or right
	float RandomRotationPitch = FMath::RandRange(-22.f, -17.f) * RecoilMultiplier; // up
	float RandomRotationYaw = FMath::RandRange(-4.f, 4.f) * RecoilMultiplier; // left/right

	FRotator RecoilRotation = FRotator(RandomRotationPitch, RandomRotationYaw, RandomRotationRoll);

	/* LOCATION (OF THE WEAPON) */
	float RandomLocationX = FMath::RandRange(-0.1f, 0.1f) * RecoilMultiplier; // forward/backward
	float RandomLocationY = FMath::RandRange(-2.f, -2.5f) * RecoilMultiplier; // left/right
	float RandomLocationZ = 0.0f; // up/down

	FVector RecoilLocation = FVector(RandomLocationX, RandomLocationY, RandomLocationZ);

	TargetRecoil = FTransform(RecoilRotation, RecoilLocation, FVector(1, 1, 1));
}


void UCPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
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
