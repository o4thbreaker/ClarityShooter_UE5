// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CSmartObject_PatrolPath.h"
#include "Components/SplineComponent.h"

ACSmartObject_PatrolPath::ACSmartObject_PatrolPath()
{
	PrimaryActorTick.bCanEverTick = false;

	Path = CreateDefaultSubobject<USplineComponent>(TEXT("Path"));
	Path->SetupAttachment(RootComponent);
	Path->bDrawDebug = true;
}

void ACSmartObject_PatrolPath::BeginPlay()
{
	Super::BeginPlay();

	GetSplinePoints();
}

void ACSmartObject_PatrolPath::GetSplinePoints()
{
	for (int i = 0; i < Path->GetNumberOfSplinePoints(); i++)
	{
		Locations.Add(Path->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
	}
}
