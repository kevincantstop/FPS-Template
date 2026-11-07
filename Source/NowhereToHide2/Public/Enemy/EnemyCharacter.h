#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class UHealthComponent;
class USkeletalMesh;
class UAnimMontage;

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
	void PlayHitReaction(FName ResolvedBone);

	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Combat")
	void ReceiveBulletHit(const FHitResult& Impact, float BaseDamage);

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Combat")
	bool IsDead() const;

	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Combat")
	bool TryAttack();

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Combat")
	bool IsAttacking() const { return bIsAttacking; }
	
	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Combat")
	float GetAttackRange() const { return AttackRange; }

	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Combat")
	void PerformAttackHitCheck();

	UHealthComponent* GetHealthComponent() const { return HealthComponent; }

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

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat", meta = (ClampMin = "1.0"))
	float HeadshotMultiplier = 3.f;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Visual")
	TArray<TObjectPtr<USkeletalMesh>> MeshPool;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	TObjectPtr<UAnimMontage> HitHeadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	TObjectPtr<UAnimMontage> HitLeftArmMontage;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	TObjectPtr<UAnimMontage> HitRightArmMontage;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	TArray<FString> HeadBoneKeywords = { TEXT("head") };

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	TArray<FString> LeftArmBoneKeywords = { TEXT("clavicle_l"), TEXT("upperarm_l"), TEXT("lowerarm_l"), TEXT("hand_l") };

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	TArray<FString> RightArmBoneKeywords = { TEXT("clavicle_r"), TEXT("upperarm_r"), TEXT("lowerarm_r"), TEXT("hand_r") };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NowhereToHide|Health", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat", meta = (ClampMin = "0.0"))
	float CorpseLifeSpan = 30.f;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat", meta = (ClampMin = "0.0"))
	float AttackRange = 150.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat", meta = (ClampMin = "0.0"))
	float AttackCooldown = 0.8f;

	UPROPERTY(VisibleInstanceOnly, Category = "NowhereToHide|Combat")
	bool bIsAttacking = false;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat", meta = (ClampMin = "0.0"))
	float AttackDamage = 15.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat", meta = (ClampMin = "1.0"))
	float AttackHitRadius = 45.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat", meta = (ClampMin = "1.0"))
	float AttackHitDistance = 110.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Combat")
	FName AttackOriginBone = TEXT("spine_03");

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	void ApplyMovementSpeed();
	void ApplyRandomMesh();
	
	float EffectiveRunSpeed = 200.f;

	float NextAttackTime = 0.f;

	UAnimMontage* SelectHitMontage(FName BoneName) const;
	FName ResolveHitBone(const FHitResult& Impact) const;

	UFUNCTION()
	void HandleDeath();
	
	void EnterRagdoll();
};