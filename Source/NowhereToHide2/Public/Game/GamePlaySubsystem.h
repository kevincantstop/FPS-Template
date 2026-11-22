#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GamePlaySubsystem.generated.h"

class AGameModeBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGamePausedChanged, bool, bPaused, bool, bShowPauseMenu);

UCLASS(Config = Game)
class NOWHERETOHIDE2_API UGamePlaySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Flow", meta = (AdvancedDisplay = "GameModeOverride"))
	void StartGame(TSubclassOf<AGameModeBase> GameModeOverride = nullptr);
	
	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Flow")
	void QuitGame();
	
	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Flow")
	void ReturnToMainMenu();

	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Flow")
	void SetGamePaused(bool bPaused, bool bShowPauseMenu = true);
	
	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Flow")
	void TogglePause();

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Flow")
	bool IsGamePaused() const;

	UPROPERTY(BlueprintAssignable, Category = "NowhereToHide|Flow")
	FOnGamePausedChanged OnGamePausedChanged;
	
protected:
	UPROPERTY(Config, EditAnywhere, Category = "Flow")
	FSoftClassPath DefaultGameplayGameMode;

private:
	void ReloadCurrentLevelWithGameMode(const FString& GameModePath) const;

	void ApplyPauseInputMode(bool bPaused);
};