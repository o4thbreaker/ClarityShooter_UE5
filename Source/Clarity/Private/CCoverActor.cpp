// Fill out your copyright notice in the Description page of Project Settings.


#include "CCoverActor.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "AI/CAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

ACCoverActor::ACCoverActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(RootComponent);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ACCoverActor::OnOverlapBegin);
	Collider->OnComponentEndOverlap.AddDynamic(this, &ACCoverActor::OnOverlapEnd);

	bIsCoverAvailable = true;
	CurrentCharacter = nullptr;
}

void ACCoverActor::BeginPlay()
{
	Super::BeginPlay();
	GetComponents<UArrowComponent>(CoverSpots, true);
}

void ACCoverActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACAICharacter* Character = Cast<ACAICharacter>(OtherActor);
	if (Character && CurrentCharacter == nullptr)
	{
		CurrentCharacter = Character;
		bIsCoverAvailable = false;
	}
}

void ACCoverActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == CurrentCharacter)
	{
		bIsCoverAvailable = true;
		CurrentCharacter = nullptr;
	}
}
