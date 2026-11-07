#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

class ACharacter;
class AEnemyCharacter;
class UCharacterMovementComponent;
class UAnimSequence;

UCLASS()
class NOWHERETOHIDE2_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Locomotion", meta = (BlueprintThreadSafe))
	UAnimSequence* GetMoveSequence() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "NowhereToHide|Locomotion", meta = (AllowPrivateAccess = "true"))
	float Speed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "NowhereToHide|Locomotion", meta = (AllowPrivateAccess = "true"))
	bool bIsMoving = false;

	UPROPERTY(BlueprintReadOnly, Category = "NowhereToHide|Locomotion", meta = (AllowPrivateAccess = "true"))
	bool bIsSprinting = false;

	UPROPERTY(BlueprintReadOnly, Category = "NowhereToHide|Locomotion", meta = (AllowPrivateAccess = "true"))
	bool bHasValidPawn = false;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Locomotion", meta = (ClampMin = "0.0"))
	float MovingSpeedThreshold = 5.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Locomotion")
	TArray<TObjectPtr<UAnimSequence>> IdleSequencePool;

	UPROPERTY(BlueprintReadOnly, Category = "NowhereToHide|Locomotion", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimSequence> IdleSequence;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NowhereToHide|Locomotion")
	TObjectPtr<UAnimSequence> RunSequence;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NowhereToHide|Locomotion")
	TObjectPtr<UAnimSequence> SprintSequence;
	
	UPROPERTY(BlueprintReadOnly, Category = "NowhereToHide|Locomotion", meta = (AllowPrivateAccess = "true"))
	float StartPosition = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "NowhereToHide|Locomotion", meta = (AllowPrivateAccess = "true"))
	bool bIsDead = false;
	
	UPROPERTY(Transient)
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(Transient)
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

private:
	UAnimSequence* PickRandomFrom(const TArray<TObjectPtr<UAnimSequence>>& Pool) const;
};