#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractableActor.h"
#include "InspectItem.generated.h"

class UUserWidget;

UCLASS()
class NOWHERETOHIDE2_API AInspectItem : public AInteractableActor
{
	GENERATED_BODY()

public:
	AInspectItem();
	
	virtual void Interact_Implementation(APawn* Interactor) override;
	
	void BeginInspect(const FTransform& TargetTransform);
	void EndInspect();
	
	void AddInspectRotation(const FVector2D& DeltaDegrees);
	void ResetInspectRotation();

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Inspect")
	bool AllowsRotation() const { return bAllowRotation; }

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Inspect")
	TSubclassOf<UUserWidget> GetInspectWidgetClass() const { return InspectWidgetClass; }

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Inspect")
	bool IsBeingInspected() const { return bInspecting; }

	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Inspect")
	FRotator InspectRotation = FRotator(0.f, 0.f, 0.f);

protected:
	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Inspect")
	bool bAllowRotation = true;
	
	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Inspect")
	TSubclassOf<UUserWidget> InspectWidgetClass;

private:
	FTransform OriginalTransform = FTransform::Identity;
	FRotator CurrentRotation = FRotator::ZeroRotator;

	bool bInspecting = false;
};