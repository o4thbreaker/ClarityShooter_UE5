#include "Weapons/CBallistics.h"

FVector CBallistics::CalculateBulletEndLocation(const FCBulletTraceParams& Params)
{
	/// \NOTE: due to floating point precision WeaponEnd can be coincided exactly with the surface point from CrosshairHitResult
		///		   and UE can sometimes not register the intersection
	FVector WeaponEnd = Params.CrosshairHitResult.bBlockingHit ? Params.CrosshairHitResult.Location : Params.End;

	FVector WeaponDirection = (WeaponEnd - Params.Start).GetSafeNormal();

	if (Params.bWillMiss)
	{
		// calculate a random miss angle within a certain range
		float MissAngleRad = FMath::DegreesToRadians(FMath::FRandRange(Params.MinMissAngle, Params.MaxMissAngle));

		// calculate direction based on the miss angle and the original direction
		FVector ScatteredDirection = FMath::VRandCone(WeaponDirection, MissAngleRad);

		// calculate the end point
		FVector AdjustedEnd = Params.Start + ScatteredDirection * Params.ShotRange;
		return AdjustedEnd;
	}
	else
	{
		/// \NOTE: because of the first NOTE we have to adjust vector by small number (10.0f) to make a difference
		FVector AdjustedEnd = Params.CrosshairHitResult.bBlockingHit ? Params.CrosshairHitResult.Location + (WeaponDirection * 10.0f) : WeaponEnd;
		return AdjustedEnd;
	}
}

