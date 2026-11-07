#include "Anims/EnemyAnimInstance.h"

#include "Animation/AnimSequence.h"
#include "Enemy/EnemyCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	MovementComponent = OwnerCharacter ? OwnerCharacter->GetCharacterMovement() : nullptr;
	bHasValidPawn = MovementComponent != nullptr;
	
	IdleSequence = PickRandomFrom(IdleSequencePool);
	StartPosition = IdleSequence
		? FMath::FRandRange(0.f, IdleSequence->GetPlayLength())
		: 0.f;
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!bHasValidPawn)
	{
		OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
		MovementComponent = OwnerCharacter ? OwnerCharacter->GetCharacterMovement() : nullptr;
		bHasValidPawn = MovementComponent != nullptr;
	}
	if (!bHasValidPawn) { return; }

	Speed = static_cast<float>(MovementComponent->Velocity.Size2D());
	bIsMoving = Speed > MovingSpeedThreshold;

	if (const AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OwnerCharacter))
	{
		bIsSprinting = Enemy->IsSprinting();
		bIsDead      = Enemy->IsDead();
	}
}

UAnimSequence* UEnemyAnimInstance::GetMoveSequence() const
{
	if (bIsSprinting && SprintSequence)
	{
		return SprintSequence;
	}
	return RunSequence;
}

UAnimSequence* UEnemyAnimInstance::PickRandomFrom(const TArray<TObjectPtr<UAnimSequence>>& Pool) const
{
	if (Pool.Num() == 0) { return nullptr; }
	return Pool[FMath::RandRange(0, Pool.Num() - 1)];
}
