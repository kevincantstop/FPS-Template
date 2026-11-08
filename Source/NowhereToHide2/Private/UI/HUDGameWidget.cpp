// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDGameWidget.h"

#include "Combat/CombatComponent.h"
#include "Health/HealthComponent.h"
#include "Player/PlayerCharacter.h"
#include "Weapon/Weapon.h"

void UHUDGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindToPawn();
	RefreshAll();
}

void UHUDGameWidget::NativeDestruct()
{
	if (CachedPlayer)
	{
		if (UHealthComponent* Health = CachedPlayer->GetHealthComponent())
		{
			Health->OnHealthChanged.RemoveDynamic(this, &UHUDGameWidget::HandleHealthChanged);
		}
		if (UCombatComponent* Combat = CachedPlayer->GetCombatComponent())
		{
			Combat->OnCurrentWeaponChanged.RemoveDynamic(this, &UHUDGameWidget::HandleCurrentWeaponChanged);
		}
	}

	BindToWeapon(nullptr);

	Super::NativeDestruct();
}

void UHUDGameWidget::BindToPawn()
{
	CachedPlayer = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (!CachedPlayer) { return; }

	if (UHealthComponent* Health = CachedPlayer->GetHealthComponent())
	{
		Health->OnHealthChanged.AddUniqueDynamic(this, &UHUDGameWidget::HandleHealthChanged);
	}

	if (UCombatComponent* Combat = CachedPlayer->GetCombatComponent())
	{
		Combat->OnCurrentWeaponChanged.AddUniqueDynamic(this, &UHUDGameWidget::HandleCurrentWeaponChanged);
		BindToWeapon(Combat->GetCurrentWeapon());
	}
}

void UHUDGameWidget::BindToWeapon(AWeapon* Weapon)
{
	if (CachedWeapon)
	{
		CachedWeapon->OnAmmoChanged.RemoveDynamic(this, &UHUDGameWidget::HandleAmmoChanged);
	}

	CachedWeapon = Weapon;

	if (CachedWeapon)
	{
		CachedWeapon->OnAmmoChanged.AddUniqueDynamic(this, &UHUDGameWidget::HandleAmmoChanged);
	}
}

void UHUDGameWidget::RefreshAll()
{
	if (!CachedPlayer) { return; }

	if (const UHealthComponent* Health = CachedPlayer->GetHealthComponent())
	{
		OnHealthUpdated(Health->GetHealth(), Health->GetMaxHealth(), 0.f);
	}

	if (CachedWeapon)
	{
		OnAmmoUpdated(CachedWeapon->GetAmmoInClip(), CachedWeapon->GetReserveAmmo());
	}
	else
	{
		OnAmmoUpdated(0, 0);
	}
}

void UHUDGameWidget::HandleHealthChanged(float NewHealth, float DeltaHealth)
{
	if (!CachedPlayer || !CachedPlayer->GetHealthComponent()) { return; }
	OnHealthUpdated(NewHealth, CachedPlayer->GetHealthComponent()->GetMaxHealth(), DeltaHealth);
}

void UHUDGameWidget::HandleAmmoChanged(int32 AmmoInClip, int32 ReserveAmmo)
{
	OnAmmoUpdated(AmmoInClip, ReserveAmmo);
}

void UHUDGameWidget::HandleCurrentWeaponChanged(AWeapon* NewWeapon)
{
	BindToWeapon(NewWeapon);
	RefreshAll();
}