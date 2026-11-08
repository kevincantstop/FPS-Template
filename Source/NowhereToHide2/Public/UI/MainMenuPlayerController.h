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
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|UI")
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|UI")
	bool bRotateCameras = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|UI", meta = (EditCondition = "bRotateCameras", ClampMin = "3.0"))
	float CameraRotateInterval = 12.f;

private:
	UPROPERTY(Transient)
	TObjectPtr<UMainMenuWidget> MainMenuWidget;

	FTimerHandle CameraRotateTimer;
};
