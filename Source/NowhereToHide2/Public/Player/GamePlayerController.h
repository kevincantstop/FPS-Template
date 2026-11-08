// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePlayerController.generated.h"

class UHUDGameWidget;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class NOWHERETOHIDE2_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AGamePlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void AcknowledgePossession(APawn* P) override;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Input")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Input")
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Input")
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Input")
	TObjectPtr<UInputAction> ReloadAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Audio")
	TObjectPtr<USoundBase> GameplayMusic;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Audio", meta = (ClampMin = "0.0", ClampMax = "2.0"))
	float GameplayMusicVolume = 0.6f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Audio", meta = (ClampMin = "0.0"))
	float GameplayMusicFadeIn = 1.5f;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|UI")
	TSubclassOf<UHUDGameWidget> HUDWidgetClass;

private:
	void HandleLook(const FInputActionValue& Value);
	void HandleMove(const FInputActionValue& Value);
	
	void HandleJumpStart();
	void HandleJumpStop();

	void HandleSprintStart();
	void HandleSprintStop();

	void HandleAimStart();
	void HandleAimStop();

	void HandleFireStart();
	void HandleFireStop();

	void HandleReload();

	void CreateHUD();
	
	UPROPERTY(Transient)
	TObjectPtr<UAudioComponent> GameplayMusicComponent;

	UPROPERTY(Transient)
	TObjectPtr<UHUDGameWidget> HUDWidget;
};
