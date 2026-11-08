#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDGameWidget.generated.h"

class APlayerCharacter;
class AWeapon;

UCLASS()
class NOWHERETOHIDE2_API UHUDGameWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "NowhereToHide|HUD")
	void OnHealthUpdated(float NewHealth, float MaxHealth, float DeltaChange);

	UFUNCTION(BlueprintImplementableEvent, Category = "NowhereToHide|HUD")
	void OnAmmoUpdated(int32 AmmoInClip, int32 ReserveAmmo);
	
	UFUNCTION()
	void HandleHealthChanged(float NewHealth, float DeltaHealth);

	UFUNCTION()
	void HandleAmmoChanged(int32 AmmoInClip, int32 ReserveAmmo);

	UFUNCTION()
	void HandleCurrentWeaponChanged(AWeapon* NewWeapon);

private:
	void BindToPawn();
	void BindToWeapon(AWeapon* Weapon);
	void RefreshAll();

	UPROPERTY(Transient)
	TObjectPtr<APlayerCharacter> CachedPlayer;

	UPROPERTY(Transient)
	TObjectPtr<AWeapon> CachedWeapon;
};