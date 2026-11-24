#include "Interaction/Inspect/InspectComponent.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Interaction/Inspect/InspectItem.h"

UInspectComponent::UInspectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = true;
}

bool UInspectComponent::BeginInspect(AInspectItem* Item)
{
	if (!Item || IsInspecting()) { return false; }

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	APlayerController* PC = Character ? Cast<APlayerController>(Character->GetController()) : nullptr;
	UWorld* World = GetWorld();
	if (!Character || !PC || !World) { return false; }

	FVector ViewLoc; FRotator ViewRot;
	PC->GetPlayerViewPoint(ViewLoc, ViewRot);

	const FTransform CamTransform(ViewRot, ViewLoc + ViewRot.Vector() * CameraForwardOffset);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	InspectionCamera = World->SpawnActor<ACameraActor>(
		ACameraActor::StaticClass(), CamTransform, Params);

	if (!InspectionCamera) { return false; }

	CurrentFOV = MaxFOV;
	if (UCameraComponent* Cam = InspectionCamera->GetCameraComponent())
	{
		Cam->SetFieldOfView(CurrentFOV);
	}

	PC->SetViewTargetWithBlend(InspectionCamera, BlendTime);

	if (UCharacterMovementComponent* Move = Character->GetCharacterMovement())
	{
		Move->DisableMovement();
	}

	const FVector ItemLoc = InspectionCamera->GetActorLocation()
		+ InspectionCamera->GetActorForwardVector() * ItemDistance;

	const FTransform ItemTransform(Item->InspectRotation, ItemLoc);
	Item->BeginInspect(ItemTransform);

	InspectingItem = Item;

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

void UInspectComponent::EndInspect()
{
	if (!IsInspecting()) { return; }

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	APlayerController* PC = Character ? Cast<APlayerController>(Character->GetController()) : nullptr;

	if (InspectingItem)
	{
		InspectingItem->EndInspect();
		InspectingItem = nullptr;
	}

	if (InspectWidget)
	{
		InspectWidget->RemoveFromParent();
		InspectWidget = nullptr;
	}

	if (PC)
	{
		PC->SetViewTargetWithBlend(Character, BlendTime);
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
	}

	if (Character)
	{
		if (UCharacterMovementComponent* Move = Character->GetCharacterMovement())
		{
			Move->SetMovementMode(MOVE_Walking);
		}
	}

	if (InspectionCamera)
	{
		InspectionCamera->Destroy();
		InspectionCamera = nullptr;
	}
}

void UInspectComponent::AddInspectRotation(const FVector2D& Delta)
{
	if (InspectingItem && InspectingItem->AllowsRotation())
	{
		InspectingItem->AddInspectRotation(Delta * RotationSensitivity);
	}
}

void UInspectComponent::AddInspectZoom(float WheelDelta)
{
	if (!InspectingItem || !InspectionCamera || FMath::IsNearlyZero(WheelDelta)) { return; }

	UCameraComponent* Cam = InspectionCamera->GetCameraComponent();
	if (!Cam) { return; }

	CurrentFOV = FMath::Clamp(CurrentFOV - WheelDelta * ZoomStep, MinFOV, MaxFOV);
	Cam->SetFieldOfView(CurrentFOV);
}

void UInspectComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	EndInspect();
	Super::EndPlay(EndPlayReason);
}