#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InspectComponent.generated.h"

class ACameraActor;
class AInspectItem;
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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Inspect", meta = (ClampMin = "0.0"))
	float CameraForwardOffset = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Inspect", meta = (ClampMin = "10.0"))
	float ItemDistance = 65.f;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Inspect")
	float MinFOV = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Inspect")
	float MaxFOV = 55.f;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Inspect")
	float ZoomStep = 3.f;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Inspect", meta = (ClampMin = "0.01"))
	float RotationSensitivity = 0.35f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Inspect", meta = (ClampMin = "0.0"))
	float BlendTime = 0.2f;

private:
	UPROPERTY(Transient)
	TObjectPtr<AInspectItem> InspectingItem;

	UPROPERTY(Transient)
	TObjectPtr<ACameraActor> InspectionCamera;

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> InspectWidget;

	float CurrentFOV = 55.f;
};