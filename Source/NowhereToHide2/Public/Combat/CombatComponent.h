// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


class UCameraComponent;
class APlayerCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOWHERETOHIDE2_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartFire();
	void StopFire();

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

private:
	UPROPERTY(Transient)
	TObjectPtr<APlayerCharacter> OwnerCharacter;

	UPROPERTY(Transient)
	TObjectPtr<UCameraComponent> Camera;

	void UpdateCameraFOV(float DeltaTime);

	void PlayFireMontage();
	UAnimMontage* SelectFireMontage() const;

	bool bIsFiring = false;
};
