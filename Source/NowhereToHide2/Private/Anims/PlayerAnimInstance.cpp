// Fill out your copyright notice in the Description page of Project Settings.


#include "Anims/PlayerAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/PlayerCharacter.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	MovementComponent = OwnerCharacter ? OwnerCharacter->GetCharacterMovement() : nullptr;
	bHasValidPawn = MovementComponent != nullptr;
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
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
	
	bIsMoving = Speed > 1.f;
	
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OwnerCharacter))
	{
		LookVector = Player->GetLookInput();
	}
	
	const FRotator SwayTarget(LookVector.Y, -LookVector.X,-LookVector.X);
	SwayRot = FMath::RInterpTo(SwayRot, SwayTarget, DeltaSeconds, SwayInterpSpeed);
}
