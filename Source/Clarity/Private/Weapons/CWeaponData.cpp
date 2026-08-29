// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CWeaponData.h"

UCWeaponData::UCWeaponData()
{
	RecoilRate = 5.0f;
	MagazineSize = 10;
	FireRate = 1.0f;
	KnockbackTime = 0.6f;
	KnockbackForce = 5000.0f;
	ShotRange = 60000.0f;
	MinMissAngle = 10.0f;
	MaxMissAngle = 35.0f;
}
