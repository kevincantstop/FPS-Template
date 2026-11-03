#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

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

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NowhereToHide|Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Weapon", meta = (ClampMin = "0.01"))
	float FireInterval = 0.12f;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Weapon")
	TObjectPtr<UAnimMontage> WeaponFireMontage;

private:
	void FireOnce();
	void PlayWeaponFireMontage();

	FTimerHandle FireTimerHandle;
	bool bFiring = false;
};
