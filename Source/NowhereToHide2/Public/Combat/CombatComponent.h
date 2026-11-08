// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


class AWeapon;
class UCameraComponent;
class APlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentWeaponChanged, AWeapon*, NewWeapon);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOWHERETOHIDE2_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();

	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Combat")
	void EquipWeapon(AWeapon* NewWeapon);

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Combat")
	AWeapon* GetCurrentWeapon() const;

	UPROPERTY(BlueprintAssignable, Category = "NowhereToHide|Combat")
	FOnCurrentWeaponChanged OnCurrentWeaponChanged;

protected:
	virtual void BeginPlay() override;

	void SpawnDefaultWeapon();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartFire();
	void StopFire();
	void OnWeaponShot();
	void OnWeaponReload();

	void Reload();
	float GetReloadMontageDuration() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Camera", meta = (ClampMin = "20.0", ClampMax = "170.0"))
	float DefaultFOV = 90.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Camera", meta = (ClampMin = "20.0", ClampMax = "170.0"))
	float AimFOV = 65.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Camera", meta = (ClampMin = "0.1"))
	float FOVInterpSpeed = 12.f;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	TObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	TObjectPtr<UAnimMontage> AimFireMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	TObjectPtr<UAnimMontage> ReloadMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	TObjectPtr<UAnimMontage> ReloadEmptyMontage;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	TSubclassOf<AWeapon> DefaultWeaponClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	FName WeaponSocketName = TEXT("R_GunSocket");
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Recoil", meta = (ClampMin = "0.0"))
	float RecoilPitchPerShot = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Recoil", meta = (ClampMin = "0.0"))
	float RecoilYawPerShot = 0.15f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Recoil", meta = (ClampMin = "0.0"))
	float AimRecoilMultiplier = 0.6f;

	UPROPERTY(Transient)
	TObjectPtr<AWeapon> CurrentWeapon;

private:
	UPROPERTY(Transient)
	TObjectPtr<APlayerCharacter> OwnerCharacter;

	UPROPERTY(Transient)
	TObjectPtr<UCameraComponent> Camera;

	void UpdateCameraFOV(float DeltaTime);

	void ApplyRecoil();

	void PlayFireMontage();
	UAnimMontage* SelectFireMontage() const;

	void PlayReloadMontage();
	UAnimMontage* SelectReloadMontage() const;

	bool bIsFiring = false;
};
