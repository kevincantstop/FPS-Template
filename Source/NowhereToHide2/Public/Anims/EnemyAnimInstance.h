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
	UPROPERTY(BlueprintReadOnly, Category = "NowhereToHide|Locomotion", meta = (AllowPrivateAccess = "true"))
	float Speed = 0.f;
	
	UPROPERTY(BlueprintReadOnly, Category = "NowhereToHide|Locomotion", meta = (AllowPrivateAccess = "true"))
	float NormalizedSpeed = 0.f;
	
	UPROPERTY(BlueprintReadOnly, Category = "NowhereToHide|Locomotion", meta = (AllowPrivateAccess = "true"))
	bool bIsMoving = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Locomotion", meta = (ClampMin = "0.0"))
	float MovingSpeedThreshold = 5.f;

	UPROPERTY(BlueprintReadOnly, Category = "NowhereToHide|Locomotion", meta = (AllowPrivateAccess = "true"))
	bool bHasValidPawn = false;

	UPROPERTY(Transient)
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(Transient)
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Locomotion")
	TArray<TObjectPtr<UAnimSequence>> IdleSequencePool;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Locomotion")
	TArray<TObjectPtr<UAnimSequence>> MoveSequencePool;
	
	UPROPERTY(BlueprintReadOnly, Category = "NowhereToHide|Locomotion", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimSequence> IdleSequence;
	
	UPROPERTY(BlueprintReadOnly, Category = "NowhereToHide|Locomotion", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimSequence> MoveSequence;
	
	UPROPERTY(BlueprintReadOnly, Category = "NowhereToHide|Locomotion", meta = (AllowPrivateAccess = "true"))
	float StartPosition = 0.f;
private:
	void PickRandomAnimations();
	UAnimSequence* PickRandomFrom(const TArray<TObjectPtr<UAnimSequence>>& Pool) const;
};