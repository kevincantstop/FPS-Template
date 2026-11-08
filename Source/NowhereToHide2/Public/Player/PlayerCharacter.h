// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UPostProcessComponent;
class UHealthComponent;
class URectLightComponent;
class UCameraComponent;
class USpringArmComponent;
class UCombatComponent;

UCLASS()
class NOWHERETOHIDE2_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	UCameraComponent* GetCamera() const { return Camera; }
	
	UCombatComponent* GetCombatComponent() const { return CombatComponent; }

	UHealthComponent* GetHealthComponent() const { return HealthComponent; }

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Movement")
	float WalkSpeed = 150.f;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Movement")
	float SprintSpeed = 450.f;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Movement", meta = (ClampMin = "0.1"))
	float SprintInterpSpeed = 8.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NowhereToHide|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	TObjectPtr<USoundBase> DeathSound;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	TSubclassOf<UCameraShakeBase> HitCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	TObjectPtr<UAnimMontage> DeathMontage;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NowhereToHide|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPostProcessComponent> HitPostProcess;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NowhereToHide|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPostProcessComponent> DeathPostProcess;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Camera", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float HitPostProcessWeight = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Camera", meta = (ClampMin = "0.1"))
	float HitPostProcessFadeSpeed = 3.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Camera", meta = (ClampMin = "0.1"))
	float DeathPostProcessFadeSpeed = 2.5f;

	virtual bool CanJumpInternal_Implementation() const override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Movement")
	void SetSprinting(bool bNewSprinting);

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Movement")
	bool IsSprinting() const { return bIsSprinting; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NowhereToHide|Movement")
	bool bJumpEnabled = true;

	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Movement")
	void SetJumpEnabled(bool bEnabled);

	void SetLookInput(const FVector2D& Axis) { LookInput = Axis; }
	
	FVector2D GetLookInput() const { return LookInput; }

	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Aim")
	void SetAiming(bool bNewAiming);

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Aim")
	bool IsAiming() const { return bIsAiming; }

	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Combat")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Combat")
	void StopFire();

	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Combat")
	void Reload();

	UFUNCTION()
	void HandleHealthChanged(float NewHealth, float DeltaHealth);

	UFUNCTION()
	void HandleDeath();
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="NowhereToHide|Camera", meta=(AllowPrivateAccess=true))
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NowhereToHide|Light", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URectLightComponent> RectLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NowhereToHide|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCombatComponent> CombatComponent;

	UPROPERTY(VisibleInstanceOnly, Category = "NowhereToHide|Aim")
	bool bIsAiming = false;

	UPROPERTY(VisibleInstanceOnly, Category = "NowhereToHide|Movement")
	bool bIsSprinting = false;

	FVector2D LookInput = FVector2D::ZeroVector;

	void ApplyMovementSpeed();

	void UpdateMovementSpeed(float DeltaTime);

	void UpdatePostProcess(float DeltaTime);
};
