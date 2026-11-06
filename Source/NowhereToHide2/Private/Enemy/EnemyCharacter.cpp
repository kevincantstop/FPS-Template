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
	
	ApplyRandomMesh();
	
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

void AEnemyCharacter::ApplyRandomMesh()
{
	if (MeshPool.Num() == 0) { return; }

	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp) { return; }

	USkeletalMesh* Picked = MeshPool[FMath::RandRange(0, MeshPool.Num() - 1)];
	if (!Picked) { return; }

	MeshComp->SetSkeletalMeshAsset(Picked);
}
