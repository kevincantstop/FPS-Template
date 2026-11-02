// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class URectLightComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class NOWHERETOHIDE2_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Movement")
	float WalkSpeed = 150.f;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Movement")
	float SprintSpeed = 450.f;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Movement", meta = (ClampMin = "0.1"))
	float SprintInterpSpeed = 8.f;

	virtual bool CanJumpInternal_Implementation() const override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="NowhereToHide|Camera", meta=(AllowPrivateAccess=true))
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NowhereToHide|Light", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URectLightComponent> RectLight;

	UPROPERTY(VisibleInstanceOnly, Category = "NowhereToHide|Movement")
	bool bIsSprinting = false;

	FVector2D LookInput = FVector2D::ZeroVector;

	void ApplyMovementSpeed();

	void UpdateMovementSpeed(float DeltaTime);
};
