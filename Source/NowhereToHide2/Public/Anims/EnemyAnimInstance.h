#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

class ACharacter;
class UCharacterMovementComponent;

UCLASS()
class NOWHERETOHIDE2_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Enemy|Locomotion", meta = (AllowPrivateAccess = "true"))
	float Speed = 0.f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Enemy|Locomotion", meta = (AllowPrivateAccess = "true"))
	float NormalizedSpeed = 0.f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Enemy|Locomotion", meta = (AllowPrivateAccess = "true"))
	bool bIsMoving = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Locomotion", meta = (ClampMin = "0.0"))
	float MovingSpeedThreshold = 5.f;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy|Locomotion", meta = (AllowPrivateAccess = "true"))
	bool bHasValidPawn = false;

	UPROPERTY(Transient)
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(Transient)
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
};