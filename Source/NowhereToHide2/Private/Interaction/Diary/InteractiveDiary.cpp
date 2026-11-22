#include "Interaction/Diary/InteractiveDiary.h"

#include "Game/GamePlaySubsystem.h"
#include "UI/DiaryInspectWidget.h"

AInteractiveDiary::AInteractiveDiary()
{
	bOneShot = false;

	PromptText = FText::FromString(TEXT("Read diary"));
}

void AInteractiveDiary::Interact_Implementation(APawn* Interactor)
{
	if (IsValid(OpenedWidget) && OpenedWidget->IsInViewport())
	{
		return;
	}

	if (!DiaryWidgetClass || !Interactor) { return; }

	APlayerController* PC = Cast<APlayerController>(Interactor->GetController());
	if (!PC) { return; }

	Super::Interact_Implementation(Interactor);
	
	if (bPauseGameWhenOpened)
	{
		if (UGameInstance* GI = GetGameInstance())
		{
			if (UGamePlaySubsystem* Flow = GI->GetSubsystem<UGamePlaySubsystem>())
			{
				Flow->SetGamePaused(true, false);
			}
		}
	}

	OpenedWidget = CreateWidget<UDiaryInspectWidget>(PC, DiaryWidgetClass);
	if (OpenedWidget)
	{
		OpenedWidget->AddToViewport(20);
		OpenedWidget->ShowDiary(DiaryID);
	}
}