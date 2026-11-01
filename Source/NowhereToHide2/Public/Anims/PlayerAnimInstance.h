// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class ACharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class NOWHERETOHIDE2_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "NowhereToHide|Locomotion", meta = (AllowPrivateAccess = "true"))
	float Speed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "NowhereToHide|Locomotion", meta = (AllowPrivateAccess = "true"))
	bool bHasValidPawn = false;

	UPROPERTY(BlueprintReadOnly, Category = "NowhereToHide|Locomotion", meta = (AllowPrivateAccess = "true"))
	bool bIsMoving = false;

	UPROPERTY(Transient)
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(Transient)
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
};
