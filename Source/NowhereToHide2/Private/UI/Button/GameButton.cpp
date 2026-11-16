#include "UI/Button/GameButton.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"

void UGameButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	if (Label && !LabelText.IsEmpty())
	{
		Label->SetText(LabelText);
	}

	ApplyIcon();
}

void UGameButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button)
	{
		Button->OnClicked.AddUniqueDynamic(this, &UGameButton::HandleClicked);
	}
}

void UGameButton::NativeDestruct()
{
	if (Button)
	{
		Button->OnClicked.RemoveDynamic(this, &UGameButton::HandleClicked);
	}

	Super::NativeDestruct();
}

void UGameButton::SetLabelText(const FText& InText)
{
	LabelText = InText;

	if (Label)
	{
		Label->SetText(LabelText);
	}
}

void UGameButton::SetIcon(UTexture2D* NewIcon)
{
	IconTexture = NewIcon;
	ApplyIcon();
}

void UGameButton::SetButtonEnabled(bool bEnabled)
{
	bIsEnabled = bEnabled;

	if (Button)
	{
		Button->SetIsEnabled(bEnabled);
	}
}

void UGameButton::HandleClicked()
{
	if (!bIsEnabled) { return; }

	OnClicked.Broadcast(this);
}

void UGameButton::ApplyIcon()
{
	if (!Icon) { return; }
	
	if (!IconTexture)
	{
		Icon->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	Icon->SetBrushFromTexture(IconTexture);
	
	Icon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}