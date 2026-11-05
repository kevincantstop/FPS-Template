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