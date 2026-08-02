// Fill out your copyright notice in the Description page of Project Settings.


#include "CAttributeComponent.h"
#include "CGameModeBase.h"
#include "CShooterInterface.h"

static TAutoConsoleVariable<bool> CVarShowHealthDebug(TEXT("art.ShowHealthDebug"), true, TEXT("Show debug info for Attributes"), ECVF_Cheat);

UCAttributeComponent::UCAttributeComponent()
{
	MaxHealth = 100.f;
	Health = MaxHealth;
	
	bShouldLimbDamage = false;

	HitZones.Add(FName("pelvis"), ECHitZone::Body);
	HitZones.Add(FName("spine_02"), ECHitZone::Body);
	HitZones.Add(FName("spine_03"), ECHitZone::Body);
	HitZones.Add(FName("spine_04"), ECHitZone::Body);
	HitZones.Add(FName("spine_05"), ECHitZone::Body);
	HitZones.Add(FName("clavicle_l"), ECHitZone::Body);
	HitZones.Add(FName("upperarm_l"), ECHitZone::Arm);
	HitZones.Add(FName("lowerarm_l"), ECHitZone::Arm);
	HitZones.Add(FName("hand_l"), ECHitZone::Hand);
	HitZones.Add(FName("clavicle_r"), ECHitZone::Body);
	HitZones.Add(FName("upperarm_r"), ECHitZone::Arm);
	HitZones.Add(FName("lowerarm_r"), ECHitZone::Arm);
	HitZones.Add(FName("hand_r"), ECHitZone::Hand);
	HitZones.Add(FName("neck_01"), ECHitZone::Body);
	HitZones.Add(FName("neck_02"), ECHitZone::Body);
	HitZones.Add(FName("head"), ECHitZone::Head);
	HitZones.Add(FName("thigh_l"), ECHitZone::Foot);
	HitZones.Add(FName("calf_l"), ECHitZone::Foot);
	HitZones.Add(FName("foot_l"), ECHitZone::Foot);
	HitZones.Add(FName("thigh_r"), ECHitZone::Foot);
	HitZones.Add(FName("calf_r"), ECHitZone::Foot);
	HitZones.Add(FName("foot_r"), ECHitZone::Foot);
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

		UE_LOG(LogTemp, Log, TEXT("%s was hit"), *HeathChangeInfo.Hit.BoneName.ToString());

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

bool UCAttributeComponent::HandleDamage(AActor* InstigatorActor, const FHealthChangeInfo& HealthChangeInfo)
{
	if (CVarShowHealthDebug.GetValueOnGameThread())
	{
		FString OuchString = FString::Printf(TEXT("OUCH: %.2f"), HealthChangeInfo.HealthDelta);
		DrawDebugString(GetWorld(), GetOwner()->GetActorLocation() + FVector(0, 10, 0), OuchString, nullptr, FColor::Red, 1.5f, true);
	}

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

ECHitZone UCAttributeComponent::GetHitZoneFromBoneName(FName BoneName) const
{
	if (const ECHitZone* Zone = HitZones.Find(BoneName))
	{
		return *Zone;
	}

	return ECHitZone::Body;
}

void UCAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, FHealthChangeInfo HeathChangeInfo)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, HeathChangeInfo);
}