// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class NOWHERETOHIDE2_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	ABullet();

	USphereComponent* GetCollisionSphere() const { return CollisionSphere; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NowhereToHide|Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NowhereToHide|Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Weapon")
	FVector DecalSize = FVector(30.f, 60.f, 60.f);

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Weapon", meta = (ClampMin = "0.0"))
	float DecalLifeSpan = 10.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Weapon")
	TObjectPtr<UMaterialInterface> DecalMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Weapon")
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Weapon")
	TObjectPtr<UParticleSystem> ImpactVFX;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Weapon", meta = (ClampMin = "0.1"))
	float MaxLifeSpan = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Weapon", meta = (ClampMin = "0.0"))
	float ImpactImpulse = 50.f;

	UFUNCTION()
	void OnProjectileStopped(const FHitResult& ImpactResult);

public:
	virtual void Tick(float DeltaTime) override;

private:
	void SpawnImpactDecal(const FHitResult& Impact);
	void PlayImpactSound(const FHitResult& Impact);
	void SpawnImpactVFX(const FHitResult& Impact);
	void ApplyImpactImpulse(const FHitResult& Impact);
};
