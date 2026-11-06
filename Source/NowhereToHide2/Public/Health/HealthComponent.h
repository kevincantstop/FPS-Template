#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, DeltaHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS(ClassGroup=(NowhereToHide), meta=(BlueprintSpawnableComponent))
class NOWHERETOHIDE2_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Health")
	void ApplyDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Health")
	void Heal(float HealAmount);

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Health")
	bool IsDead() const { return bIsDead; }

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Health")
	float GetHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Health")
	float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Health")
	float GetHealthPercent() const;
	
	UPROPERTY(BlueprintAssignable, Category = "NowhereToHide|Health")
	FOnHealthChanged OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "NowhereToHide|Health")
	FOnDeath OnDeath;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Health", meta = (ClampMin = "1.0"))
	float MaxHealth = 100.f;

	UPROPERTY(VisibleInstanceOnly, Category = "NowhereToHide|Health")
	float CurrentHealth = 0.f;

	UPROPERTY(VisibleInstanceOnly, Category = "NowhereToHide|Health")
	bool bIsDead = false;

private:
	void SetHealth(float NewHealth);
};