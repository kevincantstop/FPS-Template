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

	virtual void Tick(float DeltaSeconds) override;

	void BeginInspect(const FTransform& TargetTransform);

	void EndInspect();
	
	float BeginReturn();
	
	void AddInspectRotation(const FVector2D& DeltaDegrees);

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Inspect")
	bool AllowsRotation() const { return bAllowRotation; }

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Inspect")
	TSubclassOf<UUserWidget> GetInspectWidgetClass() const { return InspectWidgetClass; }

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Inspect")
	bool IsBeingInspected() const { return bInspecting; }

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Inspect")
	float GetInspectDistance() const { return InspectDistance; }

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Inspect")
	float GetInspectRadius() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NowhereToHide|Inspect")
	TObjectPtr<USceneComponent> InspectionPivot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NowhereToHide|Inspect")
	TObjectPtr<USceneComponent> InspectionRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NowhereToHide|Inspect")
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NowhereToHide|Inspect")
	FText InspectTitle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NowhereToHide|Inspect", meta = (MultiLine = "true"))
	FText InspectBody;
	
	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Inspect", meta = (ClampMin = "0.01"))
	FVector DesiredInspectScale = FVector(2.f);
	
	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Inspect", meta = (ClampMin = "0.05"))
	float MoveDuration = 0.6f;
	
	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Inspect")
	bool bAllowRotation = true;
	
	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Inspect")
	TSubclassOf<UUserWidget> InspectWidgetClass;
	
	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Inspect")
	float MaxPitch = 80.f;

	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Inspect", meta = (ClampMin = "0.0"))
	float InspectDistance = 0.f;

	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Inspect")
	FRotator InspectFacingOffset = FRotator(0.f, 180.f, 0.f);

private:
	void StartMove(const FTransform& Target, bool bReturning);
	FTransform ComputeNextStep(float DeltaSeconds);

	FTransform OriginalTransform = FTransform::Identity;
	FTransform MoveTarget      = FTransform::Identity;

	FRotator PivotBaseRotation   = FRotator::ZeroRotator;
	FRotator CurrentPivotRotation = FRotator::ZeroRotator;

	float MoveElapsed = 0.f;

	bool bInspecting = false;
	bool bMoving     = false;
	bool bReturning  = false;
};