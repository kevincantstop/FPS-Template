#include "Interaction/Inspect/InspectComponent.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraTypes.h"
#include "CineCameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Interaction/Inspect/InspectItem.h"

UInspectComponent::UInspectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = true;
}

void UInspectComponent::BeginPlay()
{
	Super::BeginPlay();

	CreateRuntimeCamera();
}

void UInspectComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	EndInspect();
	DestroyRuntimeCamera();

	Super::EndPlay(EndPlayReason);
}

void UInspectComponent::CreateRuntimeCamera()
{
	AActor* Owner = GetOwner();
	if (!Owner) { return; }
	
	InspectionCamera = NewObject<UCineCameraComponent>(Owner, TEXT("InspectionCamera"));
	if (!InspectionCamera) { return; }

	InspectionCamera->RegisterComponent();
	InspectionCamera->AttachToComponent(
		Owner->GetRootComponent(),
		FAttachmentTransformRules::KeepRelativeTransform);

	InspectionCamera->SetConstraintAspectRatio(false);
	
	InspectionCamera->SetActive(false);
	
	InspectionSlot = NewObject<USceneComponent>(Owner, TEXT("InspectionSlot"));
	if (InspectionSlot)
	{
		InspectionSlot->RegisterComponent();
		InspectionSlot->AttachToComponent(
			InspectionCamera, FAttachmentTransformRules::KeepRelativeTransform);
		InspectionSlot->SetRelativeLocation(FVector(ItemDistance, 0.f, 0.f));
	}
	
	if (const ACharacter* Character = Cast<ACharacter>(Owner))
	{
		PlayerCamera = Character->FindComponentByClass<UCameraComponent>();
	}
}

void UInspectComponent::DestroyRuntimeCamera()
{
	if (InspectionSlot)
	{
		InspectionSlot->DestroyComponent();
		InspectionSlot = nullptr;
	}
	if (InspectionCamera)
	{
		InspectionCamera->DestroyComponent();
		InspectionCamera = nullptr;
	}
}

bool UInspectComponent::BeginInspect(AInspectItem* Item)
{
	if (!Item || IsInspecting()) { return false; }
	if (!InspectionCamera || !InspectionSlot) { return false; }

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	APlayerController* PC = Character ? Cast<APlayerController>(Character->GetController()) : nullptr;
	if (!Character || !PC) { return false; }

	InspectingItem = Item;
	
	FVector ViewLoc;
	FRotator ViewRot;
	PC->GetPlayerViewPoint(ViewLoc, ViewRot);

	InspectionCamera->SetWorldLocationAndRotation(ViewLoc, ViewRot);
	
	if (PlayerCamera && PC->PlayerCameraManager)
	{
		InspectionCamera->SetFieldOfView(PC->PlayerCameraManager->GetFOVAngle());
	}
	
	CurrentFocalLength = FMath::Clamp(CurrentFocalLength, MinFocalLength, MaxFocalLength);
	InspectionCamera->CurrentFocalLength = CurrentFocalLength;
	FocusOnItem(Item);
	
	if (PlayerCamera)
	{
		PlayerCamera->SetActive(false);
	}
	InspectionCamera->SetActive(true);
	
	if (UCharacterMovementComponent* Move = Character->GetCharacterMovement())
	{
		Move->StopMovementImmediately();
		Move->DisableMovement();
	}

	if (bPausePlayerAnimations && Character->GetMesh())
	{
		Character->GetMesh()->bPauseAnims = true;
	}

	const float Distance = Item->GetInspectDistance() > 0.f
		? Item->GetInspectDistance()
		: ItemDistance;

	InspectionSlot->SetRelativeLocation(FVector(Distance, 0.f, 0.f));
	
	Item->BeginInspect(InspectionSlot->GetComponentTransform());
	
	if (TSubclassOf<UUserWidget> WidgetClass = Item->GetInspectWidgetClass())
	{
		InspectWidget = CreateWidget<UUserWidget>(PC, WidgetClass);
		if (InspectWidget)
		{
			InspectWidget->AddToViewport(15);
		}
	}
	
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = true;

	return true;
}

void UInspectComponent::FocusOnItem(AInspectItem* Item)
{
	if (!InspectionCamera) { return; }
	
	FCameraFocusSettings Focus;
	Focus.FocusMethod = ECameraFocusMethod::Tracking;
	Focus.TrackingFocusSettings.ActorToTrack = Item;
	Focus.TrackingFocusSettings.RelativeOffset = FVector::ZeroVector;

	InspectionCamera->FocusSettings = Focus;
}

void UInspectComponent::EndInspect()
{
	if (!IsInspecting()) { return; }

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	APlayerController* PC = Character ? Cast<APlayerController>(Character->GetController()) : nullptr;
	
	AInspectItem* Item = InspectingItem;
	InspectingItem = nullptr;

	const float ReturnDelay = Item ? Item->BeginReturn() : 0.f;

	if (Item) { Item->EndInspect(); }
	
	if (InspectWidget)
	{
		InspectWidget->RemoveFromParent();
		InspectWidget = nullptr;
	}
	
	if (UWorld* World = GetWorld())
	{
		FTimerHandle Handle;
		World->GetTimerManager().SetTimer(Handle,
			FTimerDelegate::CreateWeakLambda(this, [this, Character, PC]()
		{
			if (InspectionCamera) { InspectionCamera->SetActive(false); }
			if (PlayerCamera)     { PlayerCamera->SetActive(true); }

			if (Character)
			{
				if (UCharacterMovementComponent* Move = Character->GetCharacterMovement())
				{
					Move->SetMovementMode(MOVE_Walking);
				}
				if (Character->GetMesh())
				{
					Character->GetMesh()->bPauseAnims = false;
				}
			}

			if (PC)
			{
				PC->SetInputMode(FInputModeGameOnly());
				PC->bShowMouseCursor = false;
			}
		}), FMath::Max(ReturnDelay, 0.05f), false);
	}
}

void UInspectComponent::AddInspectRotation(const FVector2D& Delta)
{
	if (!InspectingItem || !InspectingItem->AllowsRotation()) { return; }

	InspectingItem->AddInspectRotation(Delta * RotationSensitivity);
}

void UInspectComponent::AddInspectZoom(float WheelDelta)
{
	if (!InspectingItem || !InspectionCamera || FMath::IsNearlyZero(WheelDelta)) { return; }
	
	CurrentFocalLength = FMath::Clamp(
		CurrentFocalLength + WheelDelta * ZoomStep, MinFocalLength, MaxFocalLength);

	InspectionCamera->CurrentFocalLength = CurrentFocalLength;
}