#include "Health/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	bIsDead = false;
}

void UHealthComponent::ApplyDamage(float DamageAmount)
{
	if (bIsDead || DamageAmount <= 0.f) { return; }

	SetHealth(CurrentHealth - DamageAmount);
}

void UHealthComponent::Heal(float HealAmount)
{
	if (bIsDead || HealAmount <= 0.f) { return; }

	SetHealth(CurrentHealth + HealAmount);
}

float UHealthComponent::GetHealthPercent() const
{
	return MaxHealth > 0.f ? FMath::Clamp(CurrentHealth / MaxHealth, 0.f, 1.f) : 0.f;
}

void UHealthComponent::SetHealth(float NewHealth)
{
	const float OldHealth = CurrentHealth;

	CurrentHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);

	OnHealthChanged.Broadcast(CurrentHealth, CurrentHealth - OldHealth);

	if (CurrentHealth <= 0.f && !bIsDead)
	{
		bIsDead = true;
		OnDeath.Broadcast();
	}
}