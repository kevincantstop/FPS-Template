#include "UI/MainMenuWidget.h"

#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::StartGame()
{
	const FString GameModePath = GameplayGameMode ? GameplayGameMode->GetPathName() : FString();
	if (GameModePath.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("[MainMenu] GameplayGameMode is not set"));
		return;
	}

	const FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(this, /*bRemovePrefixString*/ true);
	const FString Options = FString::Printf(TEXT("?game=%s"), *GameModePath);

	UGameplayStatics::OpenLevel(this, FName(*CurrentLevel), true, Options);
}

void UMainMenuWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}