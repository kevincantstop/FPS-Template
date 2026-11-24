#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InspectComponent.generated.h"

class AInspectItem;
class UCameraComponent;
class UCineCameraComponent;
class USceneComponent;
class UUserWidget;

UCLASS(ClassGroup=(NowhereToHide), meta=(BlueprintSpawnableComponent))
class NOWHERETOHIDE2_API UInspectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInspectComponent();

	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Inspect")
	bool BeginInspect(AInspectItem* Item);

	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Inspect")
	void EndInspect();

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Inspect")
	bool IsInspecting() const { return InspectingItem != nullptr; }
	
	void AddInspectRotation(const FVector2D& Delta);
	
	void AddInspectZoom(float WheelDelta);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Inspect", meta = (ClampMin = "10.0"))
	float ItemDistance = 44.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Inspect", meta = (ClampMin = "5.0"))
	float MinFocalLength = 35.f;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Inspect", meta = (ClampMin = "5.0"))
	float MaxFocalLength = 85.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Inspect", meta = (ClampMin = "0.1"))
	float ZoomStep = 5.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Inspect", meta = (ClampMin = "0.01"))
	float RotationSensitivity = 0.35f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Inspect", meta = (ClampMin = "12.0"))
	float InspectFocalLength = 85.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Inspect", meta = (ClampMin = "1.0"))
	float FitPadding = 1.15f;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Inspect", meta = (ClampMin = "0.0"))
	float ItemClearance = 12.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Inspect", meta = (ClampMin = "1.0"))
	float MinDistance = 22.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Inspect")
	bool bPausePlayerAnimations = true;

private:
	ACharacter*        GetOwnerCharacter() const;
	APlayerController* GetOwnerPC() const;

	void  CreateRuntimeCamera();
	void  DestroyRuntimeCamera();

	void  AlignCameraToPlayerView();

	float CalculateInspectionDistance(const AInspectItem* Item) const;

	void  FocusOnItem(AInspectItem* Item);
	void  SwitchToInspectionCamera(bool bInspect);

	void SetPlayerFrozen(bool bFrozen);
	void ApplyInspectionInputMode(bool bInspect);

	void CreateInspectionWidget(AInspectItem* Item);
	void RemoveInspectionWidget();

	void FinishEndInspect();
	
	UPROPERTY(Transient)
	TObjectPtr<UCameraComponent> PlayerCamera;
	
	UPROPERTY(Transient)
	TObjectPtr<UCineCameraComponent> InspectionCamera;
	
	UPROPERTY(Transient)
	TObjectPtr<USceneComponent> InspectionSlot;

	UPROPERTY(Transient)
	TObjectPtr<AInspectItem> InspectingItem;

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> InspectWidget;

	float CurrentFocalLength = 35.f;
};