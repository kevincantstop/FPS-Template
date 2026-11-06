// Fill out your copyright notice in the Description page of Project Settings.


#include "Anims/EnemyAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	MovementComponent = OwnerCharacter ? OwnerCharacter->GetCharacterMovement() : nullptr;
	bHasValidPawn = MovementComponent != nullptr;

	PickRandomAnimations();
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

	const float MaxSpeed = FMath::Max(MovementComponent->GetMaxSpeed(), 1.f);
	NormalizedSpeed = FMath::Clamp(Speed / MaxSpeed, 0.f, 1.f);
}

void UEnemyAnimInstance::PickRandomAnimations()
{
	IdleSequence = PickRandomFrom(IdleSequencePool);
	MoveSequence = PickRandomFrom(MoveSequencePool);
	
	StartPosition = IdleSequence
		? FMath::FRandRange(0.f, IdleSequence->GetPlayLength())
		: 0.f;
}

UAnimSequence* UEnemyAnimInstance::PickRandomFrom(const TArray<TObjectPtr<UAnimSequence>>& Pool) const
{
	if (Pool.Num() == 0) { return nullptr; }
	return Pool[FMath::RandRange(0, Pool.Num() - 1)];
}
