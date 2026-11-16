#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameButton.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UTexture2D;
class UGameButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameButtonClicked, UGameButton*, Button);

UCLASS()
class NOWHERETOHIDE2_API UGameButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "NowhereToHide|UI")
	FOnGameButtonClicked OnClicked;

	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|UI")
	void SetLabelText(const FText& InText);

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|UI")
	FText GetLabelText() const { return LabelText; }
	
	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|UI")
	void SetIcon(UTexture2D* NewIcon);

	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|UI")
	void SetButtonEnabled(bool bEnabled);

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|UI")
	bool IsButtonEnabled() const { return bIsEnabled; }

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NowhereToHide|UI")
	FText LabelText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NowhereToHide|UI")
	TObjectPtr<UTexture2D> IconTexture;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Label;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> Icon;

	UFUNCTION()
	void HandleClicked();

private:
	void ApplyIcon();

	bool bIsEnabled = true;
};