#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

UCLASS()
class NOWHERETOHIDE2_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|AI")
	float ChaseRadius = 2500.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|AI")
	float AcceptanceRadius = 90.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|AI", meta = (ClampMin = "0.05"))
	float ChaseUpdateInterval = 0.35f;

private:
	void UpdateChase();

	FTimerHandle ChaseTimer;
	bool bLoggedNavFailure = false;
};