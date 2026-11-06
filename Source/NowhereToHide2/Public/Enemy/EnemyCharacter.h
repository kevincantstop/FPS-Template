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

	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Combat")
	void PlayHitReaction(const FHitResult& Impact);

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

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Visual")
	TArray<TObjectPtr<USkeletalMesh>> MeshPool;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	TObjectPtr<UAnimMontage> HitHeadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	TObjectPtr<UAnimMontage> HitLeftArmMontage;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	TObjectPtr<UAnimMontage> HitRightArmMontage;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	TArray<FString> HeadBoneKeywords = { TEXT("head") };

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	TArray<FString> LeftArmBoneKeywords =
	{
		TEXT("clavicle_l"), TEXT("upperarm_l"), TEXT("lowerarm_l"), TEXT("hand_l")
	};

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	TArray<FString> RightArmBoneKeywords =
	{
		TEXT("clavicle_r"), TEXT("upperarm_r"), TEXT("lowerarm_r"), TEXT("hand_r")
	};

private:
	void ApplyMovementSpeed();
	void ApplyRandomMesh();
	
	float EffectiveRunSpeed = 200.f;

	UAnimMontage* SelectHitMontage(FName BoneName) const;
};