// Fill out your copyright notice in the Description page of Project Settings.


#include "CWeaponBase.h"

ACWeaponBase::ACWeaponBase()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
}

void ACWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

