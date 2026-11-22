#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DiaryInspectWidget.generated.h"

class AInspectMeshActor;
class UDiaryData;
class UImage;
class UTextBlock;
class UTextureRenderTarget2D;

UCLASS()
class NOWHERETOHIDE2_API UDiaryInspectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Inspect")
	void ShowDiary(FName DiaryID);
	
	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Inspect")
	void SetAutoRotate(bool bEnabled);

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Inspect")
	bool IsAutoRotateEnabled() const { return bAutoRotate; }

	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Inspect")
	void CloseDiary();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> PreviewImage;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> BodyText;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Inspect")
	TObjectPtr<UTextureRenderTarget2D> PreviewRenderTarget;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Inspect")
	TObjectPtr<UDiaryData> DiaryDatabase;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Inspect", meta = (ClampMin = "0.01"))
	float DragSensitivity = 0.4f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Inspect")
	float AutoRotateSpeed = 20.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Inspect", meta = (ClampMin = "1.0"))
	float CaptureRate = 20.f;

private:
	void CaptureIfNeeded(float DeltaTime);
	void DestroyPreviewActor();

	UPROPERTY(Transient)
	TObjectPtr<AInspectMeshActor> PreviewActor;

	bool bAutoRotate = false;
	bool bIsDragging = false;
	float CaptureAccumulator = 0.f;
};