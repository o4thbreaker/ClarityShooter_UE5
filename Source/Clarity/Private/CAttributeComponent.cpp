// Fill out your copyright notice in the Description page of Project Settings.


#include "CAttributeComponent.h"
#include "CGameModeBase.h"
#include "CShooterInterface.h"

static TAutoConsoleVariable<bool> CVarShowHealthDebug(TEXT("art.ShowHealthDebug"), true, TEXT("Show debug info for Attributes"), ECVF_Cheat);

UCAttributeComponent::UCAttributeComponent()
{
	MaxHealth = 100.f;
	Health = MaxHealth;
}

bool UCAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, FHealthChangeInfo& HeathChangeInfo)
{
	if (!GetOwner()->CanBeDamaged() && HeathChangeInfo.HealthDelta < 0.0f)
	{
		return false;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + HeathChangeInfo.HealthDelta, 0.0f, MaxHealth);
	float ActualDelta = NewHealth - OldHealth;

	Health = NewHealth;
	HeathChangeInfo.HealthDelta = ActualDelta;

	if (ActualDelta != 0.0f)
	{
		MulticastHealthChanged(InstigatorActor, Health, HeathChangeInfo);

		if (ActualDelta < 0.0f)
		{
			// check if owner and the shooter are in the same faction -> if so, ignore damage
			ICShooterInterface* Shooter = Cast<ICShooterInterface>(InstigatorActor);
			ICShooterInterface* OwnerShooter = Cast<ICShooterInterface>(GetOwner());
			if (Shooter && OwnerShooter && Shooter->GetFaction() == OwnerShooter->GetFaction()) return true;

			HandleDamage(InstigatorActor, HeathChangeInfo);
		}
	}

	return ActualDelta != 0.0f;
}

bool UCAttributeComponent::HandleDamage(AActor* InstigatorActor, FHealthChangeInfo HealthChangeInfo)
{
	if (CVarShowHealthDebug.GetValueOnGameThread())
	{
		FString OuchString = FString::Printf(TEXT("OUCH: %.2f"), HealthChangeInfo.HealthDelta);
		DrawDebugString(GetWorld(), GetOwner()->GetActorLocation() + FVector(0, 10, 0), OuchString, nullptr, FColor::Red, 1.5f, true);
	}

	/// \TODO: make replicated
	OnDamage.Broadcast(InstigatorActor, this, Health, HealthChangeInfo);

	// died
	if (Health <= 0.0f)
	{
		ACGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ACGameModeBase>();
		if (GameMode)
		{
			GameMode->OnActorKilled(GetOwner(), InstigatorActor);
		}

		return true;
	}

	return true;
}

bool UCAttributeComponent::Kill(AActor* InstigatorActor)
{
	FHealthChangeInfo HealthChangeInfo;
	HealthChangeInfo.HealthDelta = -MaxHealth;

	return ApplyHealthChange(InstigatorActor, HealthChangeInfo);
}

/// \TODO: change to Interface call
UCAttributeComponent* UCAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UCAttributeComponent>(FromActor->GetComponentByClass(UCAttributeComponent::StaticClass()));
	}

	return nullptr;
}

void UCAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, FHealthChangeInfo HeathChangeInfo)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, HeathChangeInfo);
}