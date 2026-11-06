#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class NOWHERETOHIDE2_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Movement")
	void SetSprinting(bool bNewSprinting);

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Movement")
	bool IsSprinting() const { return bIsSprinting; }

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Movement")
	bool CanSprint() const { return bCanSprint; }

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Movement", meta = (ClampMin = "0.0"))
	float RunSpeed = 200.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Movement", meta = (ClampMin = "0.0"))
	float SprintSpeed = 480.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Movement", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float SprintChance = 0.3f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Movement", meta = (ClampMin = "0.0"))
	float SpeedVariation = 25.f;
	
	UPROPERTY(VisibleInstanceOnly, Category = "NowhereToHide|Movement")
	bool bIsSprinting = false;
	
	UPROPERTY(VisibleInstanceOnly, Category = "NowhereToHide|Movement")
	bool bCanSprint = false;

private:
	void ApplyMovementSpeed();
	
	float EffectiveRunSpeed = 200.f;
};