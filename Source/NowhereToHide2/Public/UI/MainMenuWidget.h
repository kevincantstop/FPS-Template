// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class NOWHERETOHIDE2_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|UI")
	void StartGame();
	
	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|UI")
	void QuitGame();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|UI")
	TSubclassOf<AGameModeBase> GameplayGameMode;
};
