// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetMesh());
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bEnableCameraLag = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;

	GetMesh()->CastShadow = false;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	ApplyMovementSpeed();
}

bool APlayerCharacter::CanJumpInternal_Implementation() const
{
	return bJumpEnabled && Super::CanJumpInternal_Implementation();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateMovementSpeed(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerCharacter::SetSprinting(bool bNewSprinting)
{
	if (bIsSprinting == bNewSprinting) { return; }

	bIsSprinting = bNewSprinting;
}

void APlayerCharacter::SetJumpEnabled(bool bEnabled)
{
	if (bJumpEnabled == bEnabled) { return; }

	bJumpEnabled = bEnabled;
	
	if (!bJumpEnabled)
	{
		StopJumping();
	}
}

void APlayerCharacter::ApplyMovementSpeed()
{
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = bIsSprinting ? SprintSpeed : WalkSpeed;
	}
}

void APlayerCharacter::UpdateMovementSpeed(float DeltaTime)
{
	UCharacterMovementComponent* Move = GetCharacterMovement();
	if (!Move) { return; }

	const float TargetSpeed = bIsSprinting ? SprintSpeed : WalkSpeed;
	
	Move->MaxWalkSpeed = FMath::FInterpTo(Move->MaxWalkSpeed, TargetSpeed, DeltaTime, SprintInterpSpeed);
}

