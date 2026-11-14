#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"

class AActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionTargetChanged);

UCLASS(ClassGroup=(NowhereToHide), meta=(BlueprintSpawnableComponent))
class NOWHERETOHIDE2_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractComponent();
	
	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Interaction")
	bool TryInteract();

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Interaction")
	AActor* GetFocusedActor() const { return FocusedActor; }

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Interaction")
	FText GetInteractionPrompt() const { return InteractionPrompt; }

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Interaction")
	bool HasFocusedActor() const { return FocusedActor != nullptr; }
	
	UPROPERTY(BlueprintAssignable, Category = "NowhereToHide|Interaction")
	FOnInteractionTargetChanged OnInteractionTargetChanged;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Interaction", meta = (ClampMin = "0.0"))
	float InteractionRange = 250.f;
	
	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Interaction", meta = (ClampMin = "0.0"))
	float UpdateInterval = 0.f;

private:
	void UpdateInteractionTarget();
	void SetFocusedActor(AActor* NewActor);

	UPROPERTY(Transient)
	TObjectPtr<AActor> FocusedActor;

	FText InteractionPrompt;

	float TimeSinceLastCheck = 0.f;
};