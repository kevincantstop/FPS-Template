#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InspectMeshActor.generated.h"

class UPointLightComponent;

UCLASS()
class NOWHERETOHIDE2_API AInspectMeshActor : public AActor
{
	GENERATED_BODY()

public:
	AInspectMeshActor();

	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Inspect")
	void SetPreviewMesh(UStaticMesh* InMesh, const FRotator& InitialRotation);
	
	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Inspect")
	void AddPreviewRotation(float DeltaYaw, float DeltaPitch);
	
	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Inspect")
	void ResetRotation();
	
	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Inspect")
	void CaptureNow();

protected:
	UPROPERTY(VisibleAnywhere, Category = "NowhereToHide|Inspect")
	TObjectPtr<UStaticMeshComponent> PreviewMesh;

	UPROPERTY(VisibleAnywhere, Category = "NowhereToHide|Inspect")
	TObjectPtr<USceneCaptureComponent2D> Capture;

	UPROPERTY(VisibleAnywhere, Category = "NowhereToHide|Inspect")
	TObjectPtr<UPointLightComponent> KeyLight;

	UPROPERTY(VisibleAnywhere, Category = "NowhereToHide|Inspect")
	TObjectPtr<USkyLightComponent> FillLight;

private:
	FRotator BaseRotation = FRotator::ZeroRotator;
	
	float CurrentYaw = 0.f;
	
	float CurrentPitch = 0.f;
};
