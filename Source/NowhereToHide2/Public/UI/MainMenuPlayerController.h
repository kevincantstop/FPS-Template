// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuPlayerController.generated.h"

class UMainMenuWidget;
/**
 * 
 */
UCLASS()
class NOWHERETOHIDE2_API AMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|UI")
	void UseRandomCamera();

	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Audio")
	void StopMenuMusic(float FadeOutTime = 1.f);
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|UI")
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|UI")
	bool bRotateCameras = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|UI", meta = (EditCondition = "bRotateCameras", ClampMin = "3.0"))
	float CameraRotateInterval = 12.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|UI")
	TSubclassOf<UCameraShakeBase> CameraIdleShake;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|UI", meta = (ClampMin = "0.0"))
	float CameraIdleShakeScale = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Audio")
	TObjectPtr<USoundBase> MenuMusic;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Audio", meta = (ClampMin = "0.0", ClampMax = "2.0"))
	float MenuMusicVolume = 0.6f;

private:
	UPROPERTY(Transient)
	TObjectPtr<UMainMenuWidget> MainMenuWidget;

	UPROPERTY(Transient)
	TObjectPtr<UAudioComponent> MenuMusicComponent;

	FTimerHandle CameraRotateTimer;
};
