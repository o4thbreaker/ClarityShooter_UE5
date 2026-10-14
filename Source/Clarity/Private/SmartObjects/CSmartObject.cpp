// Fill out your copyright notice in the Description page of Project Settings.


#include "SmartObjects/CSmartObject.h"
#include "Components/BillboardComponent.h"
#include "Components/ArrowComponent.h"

ACSmartObject::ACSmartObject()
{
	PrimaryActorTick.bCanEverTick = false;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	SetRootComponent(Billboard);

	FacingDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("FacingDirection"));
	FacingDirection->SetupAttachment(Billboard);
}

