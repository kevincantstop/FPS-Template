#include "Game/GamePlaySubsystem.h"

#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"

void UGamePlaySubsystem::StartGame(TSubclassOf<AGameModeBase> GameModeOverride)
{
	UClass* GameModeClass = GameModeOverride
		? GameModeOverride.Get()
		: DefaultGameplayGameMode.TryLoadClass<AGameModeBase>();

	if (!GameModeClass)
	{
		UE_LOG(LogTemp, Error, TEXT("[GamePlay] No GameMode specified"));
		return;
	}

	ReloadCurrentLevelWithGameMode(GameModeClass->GetPathName());
}

void UGamePlaySubsystem::QuitGame()
{
	if (UWorld* World = GetWorld())
	{
		UKismetSystemLibrary::QuitGame(
			World, World->GetFirstPlayerController(), EQuitPreference::Quit, false);
	}
}

void UGamePlaySubsystem::ReturnToMainMenu()
{
	ReloadCurrentLevelWithGameMode(FString());
}

void UGamePlaySubsystem::ReloadCurrentLevelWithGameMode(const FString& GameModePath) const
{
	UWorld* World = GetWorld();
	if (!World) { return; }

	const FString LevelName = UGameplayStatics::GetCurrentLevelName(World, /*bRemovePrefixString*/ true);

	const FString Options = GameModePath.IsEmpty()
		? FString()
		: FString::Printf(TEXT("?game=%s"), *GameModePath);

	UGameplayStatics::OpenLevel(World, FName(*LevelName), true, Options);
}

void UGamePlaySubsystem::SetGamePaused(bool bPaused)
{
	UWorld* World = GetWorld();
	if (!World) { return; }

	UGameplayStatics::SetGamePaused(World, bPaused);

	ApplyPauseInputMode(bPaused);

	OnGamePausedChanged.Broadcast(bPaused);
}

void UGamePlaySubsystem::TogglePause()
{
	SetGamePaused(!IsGamePaused());
}

bool UGamePlaySubsystem::IsGamePaused() const
{
	const UWorld* World = GetWorld();
	return World && UGameplayStatics::IsGamePaused(World);
}

void UGamePlaySubsystem::ApplyPauseInputMode(bool bPaused)
{
	APlayerController* PC = GetGameInstance()
		? GetGameInstance()->GetFirstLocalPlayerController()
		: nullptr;

	if (!PC) { return; }

	if (bPaused)
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);

		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}
	else
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
	}
}