#pragma once

struct FCBulletTraceParams
{
	FVector Start;
	FVector End;
	FHitResult CrosshairHitResult;
	bool bWillMiss = false;
	float ShotRange = 0.0f;
	float MinMissAngle = 10.0f;
	float MaxMissAngle = 35.0f;
};

namespace CBallistics
{
	FVector CalculateBulletEndLocation(const FCBulletTraceParams& Params);
}