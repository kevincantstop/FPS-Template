// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GamePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/AudioComponent.h"
#include "Game/GamePlaySubsystem.h"
#include "GameFramework/Character.h"
#include "Interaction/InteractComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"
#include "UI/HUDGameWidget.h"

AGamePlayerController::AGamePlayerController()
{
	bShowMouseCursor = false;
	bShouldPerformFullTickWhenPaused = true;
}

void AGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
	SetIgnoreMoveInput(false);
	SetIgnoreLookInput(false);

	if (PlayerCameraManager)
	{
		PlayerCameraManager->ViewPitchMin = -85.f;
		PlayerCameraManager->ViewPitchMax =  85.f;
	}

	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (auto* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}

	if (GameplayMusic)
	{
		GameplayMusicComponent = UGameplayStatics::SpawnSound2D(this, GameplayMusic, GameplayMusicVolume);
		if (GameplayMusicComponent)
		{
			GameplayMusicComponent->FadeIn(GameplayMusicFadeIn, 1.f);
		}
	}

	if (UGamePlaySubsystem* Flow = GetGameInstance()->GetSubsystem<UGamePlaySubsystem>())
	{
		Flow->OnGamePausedChanged.AddUniqueDynamic(this, &AGamePlayerController::HandleGamePausedChanged);
	}
}

void AGamePlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UGamePlaySubsystem* Flow = GetGameInstance() ? GetGameInstance()->GetSubsystem<UGamePlaySubsystem>() : nullptr)
	{
		Flow->OnGamePausedChanged.RemoveDynamic(this, &AGamePlayerController::HandleGamePausedChanged);
		
		if (Flow->IsGamePaused())
		{
			Flow->SetGamePaused(false);
		}
	}
	
	Super::EndPlay(EndPlayReason);
}

void AGamePlayerController::HandleGamePausedChanged(bool bPaused)
{
	ShowPauseMenu(bPaused);
	SwapInputMappingContext(bPaused);
}

void AGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGamePlayerController::HandleMove);
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGamePlayerController::HandleLook);
		
		EIC->BindAction(JumpAction, ETriggerEvent::Started,   this, &AGamePlayerController::HandleJumpStart);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &AGamePlayerController::HandleJumpStop);
		
		EIC->BindAction(SprintAction, ETriggerEvent::Started,   this, &AGamePlayerController::HandleSprintStart);
		EIC->BindAction(SprintAction, ETriggerEvent::Completed, this, &AGamePlayerController::HandleSprintStop);

		EIC->BindAction(AimAction, ETriggerEvent::Started,   this, &AGamePlayerController::HandleAimStart);
		EIC->BindAction(AimAction, ETriggerEvent::Completed, this, &AGamePlayerController::HandleAimStop);

		EIC->BindAction(FireAction, ETriggerEvent::Started,   this, &AGamePlayerController::HandleFireStart);
		EIC->BindAction(FireAction, ETriggerEvent::Completed, this, &AGamePlayerController::HandleFireStop);

		EIC->BindAction(ReloadAction, ETriggerEvent::Started, this, &AGamePlayerController::HandleReload);

		EIC->BindAction(InteractAction, ETriggerEvent::Started, this, &AGamePlayerController::HandleInteract);
		
		EIC->BindAction(PauseAction, ETriggerEvent::Started, this, &AGamePlayerController::HandlePause);
	}
}

void AGamePlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	CreateHUD();
}

void AGamePlayerController::HandleLook(const FInputActionValue& Value)
{
	if (!IsGameInputAllowed()) { return; }
	
	const FVector2D Axis = Value.Get<FVector2D>();
	AddYawInput(Axis.X);
	AddPitchInput(-Axis.Y);

	if (APlayerCharacter* C = Cast<APlayerCharacter>(GetPawn()))
	{
		C->SetLookInput(Axis);
	}
}

void AGamePlayerController::HandleMove(const FInputActionValue& Value)
{
	if (!IsGameInputAllowed()) { return; }
	
	APawn* P = GetPawn();
	if (!P) { return; }

	const FVector2D Axis = Value.Get<FVector2D>();
	
	const FRotator YawOnly(0.f, GetControlRotation().Yaw, 0.f);
	const FVector Forward = FRotationMatrix(YawOnly).GetUnitAxis(EAxis::X);
	const FVector Right   = FRotationMatrix(YawOnly).GetUnitAxis(EAxis::Y);

	P->AddMovementInput(Forward, Axis.X);
	P->AddMovementInput(Right,   Axis.Y);
}

void AGamePlayerController::HandleJumpStart()
{
	if (!IsGameInputAllowed()) { return; }
	
	if (ACharacter* C = Cast<ACharacter>(GetPawn()))
	{
		if (C->CanJump())
		{
			C->Jump();
		}
	}
}

void AGamePlayerController::HandleJumpStop()
{
	if (ACharacter* C = Cast<ACharacter>(GetPawn()))
	{
		C->StopJumping();
	}
}

void AGamePlayerController::HandleSprintStart()
{
	if (!IsGameInputAllowed()) { return; }
	
	if (APlayerCharacter* C = Cast<APlayerCharacter>(GetPawn()))
	{
		C->SetSprinting(true);
	}
}

void AGamePlayerController::HandleSprintStop()
{
	if (APlayerCharacter* C = Cast<APlayerCharacter>(GetPawn()))
	{
		C->SetSprinting(false);
	}
}

void AGamePlayerController::HandleAimStart()
{
	if (!IsGameInputAllowed()) { return; }
	
	if (APlayerCharacter* C = Cast<APlayerCharacter>(GetPawn()))
	{
		C->SetAiming(true);
	}
}

void AGamePlayerController::HandleAimStop()
{
	if (APlayerCharacter* C = Cast<APlayerCharacter>(GetPawn()))
	{
		C->SetAiming(false);
	}
}

void AGamePlayerController::HandleFireStart()
{
	if (!IsGameInputAllowed()) { return; }
	
	if (APlayerCharacter* C = Cast<APlayerCharacter>(GetPawn()))
	{
		C->StartFire();
	}
}

void AGamePlayerController::HandleFireStop()
{
	if (APlayerCharacter* C = Cast<APlayerCharacter>(GetPawn()))
	{
		C->StopFire();
	}
}

void AGamePlayerController::HandleReload()
{
	if (!IsGameInputAllowed()) { return; }
	
	if (APlayerCharacter* C = Cast<APlayerCharacter>(GetPawn()))
	{
		C->Reload();
	}
}

void AGamePlayerController::HandleInteract()
{
	if (!IsGameInputAllowed()) { return; }
	
	if (const APlayerCharacter* C = Cast<APlayerCharacter>(GetPawn()))
	{
		if (UInteractComponent* Interact = C->GetInteractComponent())
		{
			Interact->TryInteract();
		}
	}
}

void AGamePlayerController::CreateHUD()
{
	if (!HUDWidgetClass || HUDWidget) { return; }

	HUDWidget = CreateWidget<UHUDGameWidget>(this, HUDWidgetClass);
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
	}
}

void AGamePlayerController::HandlePause()
{
	if (UGamePlaySubsystem* Flow = GetGameInstance()->GetSubsystem<UGamePlaySubsystem>())
	{
		Flow->TogglePause();
	}
}

bool AGamePlayerController::IsGameInputAllowed() const
{
	const UWorld* World = GetWorld();
	return World && !UGameplayStatics::IsGamePaused(World);
}

void AGamePlayerController::SwapInputMappingContext(bool bPaused)
{
	ULocalPlayer* LP = GetLocalPlayer();
	if (!LP) { return; }

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!Subsystem) { return; }

	if (bPaused)
	{
		if (DefaultMappingContext)
		{
			Subsystem->RemoveMappingContext(DefaultMappingContext);
		}
		if (PauseMappingContext)
		{
			Subsystem->AddMappingContext(PauseMappingContext, 0);
		}
	}
	else
	{
		if (PauseMappingContext)
		{
			Subsystem->RemoveMappingContext(PauseMappingContext);
		}

		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AGamePlayerController::ShowPauseMenu(bool bShow)
{
	if (bShow)
	{
		if (!PauseMenuWidget && PauseMenuClass)
		{
			PauseMenuWidget = CreateWidget<UUserWidget>(this, PauseMenuClass);
		}

		if (PauseMenuWidget)
		{
			PauseMenuWidget->AddToViewport(10);
		}
	}
	else if (PauseMenuWidget)
	{
		PauseMenuWidget->RemoveFromParent();
	}
}
