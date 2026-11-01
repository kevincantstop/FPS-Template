// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePlayerController.generated.h"

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

private:
	void HandleLook(const FInputActionValue& Value);
	void HandleMove(const FInputActionValue& Value);
	
	void HandleJumpStart();
	void HandleJumpStop();

	void HandleSprintStart();
	void HandleSprintStop();
};
