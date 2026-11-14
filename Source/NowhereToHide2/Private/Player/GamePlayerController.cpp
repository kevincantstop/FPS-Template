// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GamePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Character.h"
#include "Interaction/InteractComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"
#include "UI/HUDGameWidget.h"

AGamePlayerController::AGamePlayerController()
{
	bShowMouseCursor = false;
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
	}
}

void AGamePlayerController::AcknowledgePossession(class APawn* P)
{
	Super::AcknowledgePossession(P);

	CreateHUD();
}

void AGamePlayerController::HandleLook(const FInputActionValue& Value)
{
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
	if (APlayerCharacter* C = Cast<APlayerCharacter>(GetPawn()))
	{
		C->Reload();
	}
}

void AGamePlayerController::HandleInteract()
{
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
