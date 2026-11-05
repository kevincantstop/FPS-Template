#include "Enemy/EnemyAIController.h"
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
	
	if (DistSq > FMath::Square(ChaseRadius))
	{
		if (GetMoveStatus() == EPathFollowingStatus::Moving)
		{
			StopMovement();
		}
		return;
	}
	
	if (DistSq <= FMath::Square(AcceptanceRadius))
	{
		return;
	}

	const EPathFollowingRequestResult::Type Result = MoveToActor(Player, AcceptanceRadius);

	if (Result == EPathFollowingRequestResult::Failed && !bLoggedNavFailure)
	{
		bLoggedNavFailure = true;
		UE_LOG(LogTemp, Warning,
			TEXT("[Enemy] MoveTo failed for %s. Check that a NavMeshBoundsVolume covers both the AI and the player."),
			*GetNameSafe(MyPawn));
	}
}