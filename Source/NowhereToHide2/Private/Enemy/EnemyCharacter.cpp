#include "Enemy/EnemyCharacter.h"

#include "Enemy/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI    = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	
	GetCharacterMovement()->bUseRVOAvoidance = true;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	bCanSprint = FMath::FRand() < SprintChance;
	
	EffectiveRunSpeed = RunSpeed + FMath::FRandRange(-SpeedVariation, SpeedVariation);

	SetSprinting(bCanSprint);
	
	ApplyMovementSpeed();
}

void AEnemyCharacter::SetSprinting(bool bNewSprinting)
{
	if (!bCanSprint)
	{
		bNewSprinting = false;
	}

	if (bIsSprinting == bNewSprinting) { return; }

	bIsSprinting = bNewSprinting;
	ApplyMovementSpeed();
}

void AEnemyCharacter::ApplyMovementSpeed()
{
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = bIsSprinting ? SprintSpeed : EffectiveRunSpeed;
	}
}