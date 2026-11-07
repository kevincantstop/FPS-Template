#include "Enemy/EnemyAIController.h"

#include "Enemy/EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	GetWorldTimerManager().SetTimer(
		ChaseTimer, this, &AEnemyAIController::UpdateChase, ChaseUpdateInterval, /*bLoop*/ true, 0.f);
}

void AEnemyAIController::OnUnPossess()
{
	GetWorldTimerManager().ClearTimer(ChaseTimer);
	Super::OnUnPossess();
}

void AEnemyAIController::UpdateChase()
{
	APawn* MyPawn = GetPawn();
	if (!MyPawn) { return; }

	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Player) { return; }

	const float DistSq = FVector::DistSquared2D(
		Player->GetActorLocation(), MyPawn->GetActorLocation());

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(MyPawn);
	if (!Enemy || Enemy->IsDead()) { return; }
	
	if (DistSq <= FMath::Square(Enemy->GetAttackRange()))
	{
		if (GetMoveStatus() == EPathFollowingStatus::Moving)
		{
			StopMovement();
		}
		
		const FVector ToPlayer = Player->GetActorLocation() - MyPawn->GetActorLocation();
		MyPawn->SetActorRotation(FRotator(0.f, ToPlayer.Rotation().Yaw, 0.f));

		Enemy->TryAttack();
		return;
	}
	
	if (DistSq > FMath::Square(ChaseRadius))
	{
		if (GetMoveStatus() == EPathFollowingStatus::Moving) { StopMovement(); }
		return;
	}

	MoveToActor(Player, AcceptanceRadius);
}