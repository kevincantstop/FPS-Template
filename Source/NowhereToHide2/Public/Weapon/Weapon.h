#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class ABullet;

UCLASS()
class NOWHERETOHIDE2_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

	USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	
	void AttachToMesh(USkeletalMeshComponent* ParentMesh, FName SocketName);

	void StartFire();
	void StopFire();

	bool IsFiring() const { return bFiring; }
	float GetFireInterval() const { return FireInterval; }

	int GetAmmoInClip() const { return AmmoInClip; }
	int GetReserveAmmo() const { return ReserveAmmo; }
	bool HasAmmo() const { return AmmoInClip > 0; }

	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Weapon")
	void StartReload();
	bool IsReloading() const { return bReloading; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NowhereToHide|Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Weapon", meta = (ClampMin = "0.01"))
	float FireInterval = 0.12f;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Weapon")
	TObjectPtr<UAnimMontage> WeaponFireMontage;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Weapon")
	TSubclassOf<ABullet> BulletClass;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Weapon")
	FName MuzzleSocketName = TEXT("Muzzle");

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Weapon", meta = (ClampMin = "1"))
	int32 ClipSize = 10;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Weapon", meta = (ClampMin = "0"))
	int32 ReserveAmmo = 200;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Weapon")
	TObjectPtr<UAnimMontage> ReloadMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Weapon")
	TObjectPtr<UAnimMontage> ReloadEmptyMontage;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Weapon")
	TObjectPtr<USoundBase> DryFireSound;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Weapon", meta = (ClampMin = "0.0", UIMax = "10.0"))
	float HipSpreadDegrees = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Weapon", meta = (ClampMin = "0.0", UIMax = "10.0"))
	float AimSpreadDegrees = 0.15f;
	
	UPROPERTY(VisibleInstanceOnly, Category = "NowhereToHide|Weapon")
	int32 AmmoInClip = 0;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Weapon", meta = (ClampMin = "0.1"))
	float ReloadFallbackDuration = 2.f;
	
private:
	void FireBullet();
	void FireOnce();
	void PlayWeaponFireMontage();
	void PlayDryFireSound();
	float GetCurrentSpreadDegrees() const;

	void PlayReloadMontage();
	UAnimMontage* SelectReloadMontage() const;

	FTimerHandle FireTimerHandle;
	bool bFiring = false;

	void FinishReload();
	float GetReloadDuration() const;

	FTimerHandle ReloadTimerHandle;
	bool bReloading = false;
};
