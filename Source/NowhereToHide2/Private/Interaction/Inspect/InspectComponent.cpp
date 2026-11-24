#include "Interaction/Inspect/InspectComponent.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraTypes.h"
#include "CineCameraComponent.h"
#include "Combat/CombatComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Interaction/Inspect/InspectItem.h"
#include "Player/PlayerCharacter.h"
#include "Weapon/Weapon.h"

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

ACharacter* UInspectComponent::GetOwnerCharacter() const
{
	return Cast<ACharacter>(GetOwner());
}

APlayerController* UInspectComponent::GetOwnerPC() const
{
	const ACharacter* Character = GetOwnerCharacter();
	return Character ? Cast<APlayerController>(Character->GetController()) : nullptr;
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

void UInspectComponent::AlignCameraToPlayerView()
{
	APlayerController* PC = GetOwnerPC();
	if (!PC || !InspectionCamera) { return; }
	
	FVector  ViewLoc;
	FRotator ViewRot;
	PC->GetPlayerViewPoint(ViewLoc, ViewRot);

	InspectionCamera->SetWorldLocationAndRotation(ViewLoc, ViewRot);

	if (PlayerCamera && PC->PlayerCameraManager)
	{
		InspectionCamera->SetFieldOfView(PC->PlayerCameraManager->GetFOVAngle());
	}

	CurrentFocalLength = FMath::Clamp(CurrentFocalLength, MinFocalLength, MaxFocalLength);
	InspectionCamera->CurrentFocalLength = CurrentFocalLength;
}

float UInspectComponent::CalculateInspectionDistance(const AInspectItem* Item) const
{
	const float DesiredDistance = (Item && Item->GetInspectDistance() > 0.f)
		? Item->GetInspectDistance()
		: ItemDistance;

	float Distance = FMath::Max(DesiredDistance, MinDistance);
	
	const UWorld* World = GetWorld();
	if (!World || !InspectionCamera) { return Distance; }

	const FVector Start = InspectionCamera->GetComponentLocation();
	const FVector End   = Start + InspectionCamera->GetForwardVector() * Distance;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(InspectOcclusion), false, GetOwner());
	Params.AddIgnoredActor(GetOwner());
	if (Item) { Params.AddIgnoredActor(Item); }

	FHitResult Hit;
	const bool bBlocked = World->SweepSingleByChannel(
		Hit, Start, End, FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(FMath::Max(ItemClearance, 1.f)),
		Params);

	if (bBlocked)
	{
		Distance = FMath::Max(Hit.Distance - ItemClearance, MinDistance);
	}

	return Distance;
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

void UInspectComponent::SwitchToInspectionCamera(bool bInspect)
{
	if (!InspectionCamera) { return; }

	if (bInspect)
	{
		if (PlayerCamera) { PlayerCamera->SetActive(false); }
		InspectionCamera->SetActive(true);
	}
	else
	{
		InspectionCamera->SetActive(false);
		if (PlayerCamera) { PlayerCamera->SetActive(true); }
	}
}

void UInspectComponent::SetPlayerFrozen(bool bFrozen)
{
	ACharacter* Character = GetOwnerCharacter();
	if (!Character) { return; }

	if (UCharacterMovementComponent* Move = Character->GetCharacterMovement())
	{
		if (bFrozen)
		{
			Move->StopMovementImmediately();
			Move->DisableMovement();
		}
		else
		{
			Move->SetMovementMode(MOVE_Walking);
		}
	}

	if (bPausePlayerAnimations && Character->GetMesh())
	{
		Character->GetMesh()->bPauseAnims = bFrozen;
	}
}

void UInspectComponent::ApplyInspectionInputMode(bool bInspect)
{
	APlayerController* PC = GetOwnerPC();
	if (!PC) { return; }

	if (bInspect)
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);

		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}
	else
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
	}
}

void UInspectComponent::CreateInspectionWidget(AInspectItem* Item)
{
	APlayerController* PC = GetOwnerPC();
	if (!PC || !Item) { return; }

	const TSubclassOf<UUserWidget> WidgetClass = Item->GetInspectWidgetClass();
	if (!WidgetClass) { return; }

	InspectWidget = CreateWidget<UUserWidget>(PC, WidgetClass);
	if (InspectWidget)
	{
		InspectWidget->AddToViewport(15);
	}
}

void UInspectComponent::RemoveInspectionWidget()
{
	if (InspectWidget)
	{
		InspectWidget->RemoveFromParent();
		InspectWidget = nullptr;
	}
}

bool UInspectComponent::BeginInspect(AInspectItem* Item)
{
	if (!Item || IsInspecting()) { return false; }
	if (!InspectionCamera || !InspectionSlot) { return false; }
	if (!GetOwnerCharacter() || !GetOwnerPC() || !GetWorld()) { return false; }

	InspectingItem = Item;
	
	AlignCameraToPlayerView();
	FocusOnItem(Item);

	const float Distance = CalculateInspectionDistance(Item);
	InspectionSlot->SetRelativeLocation(FVector(Distance, 0.f, 0.f));
	
	SwitchToInspectionCamera(true);
	SetPlayerFrozen(true);
	SetPlayerVisualsHidden(true);
	
	Item->BeginInspect(InspectionSlot->GetComponentTransform());
	
	CreateInspectionWidget(Item);
	ApplyInspectionInputMode(true);

	return true;
}

void UInspectComponent::EndInspect()
{
	if (!IsInspecting()) { return; }

	AInspectItem* Item = InspectingItem;
	InspectingItem = nullptr;
	
	const float ReturnDelay = Item ? Item->BeginReturn() : 0.f;
	if (Item) { Item->EndInspect(); }

	RemoveInspectionWidget();
	
	UWorld* World = GetWorld();
	if (!World)
	{
		FinishEndInspect();
		return;
	}

	FTimerHandle Handle;
	World->GetTimerManager().SetTimer(
		Handle,
		FTimerDelegate::CreateWeakLambda(this, [this]() { FinishEndInspect(); }),
		FMath::Max(ReturnDelay, 0.05f),
		false);
}

void UInspectComponent::FinishEndInspect()
{
	SwitchToInspectionCamera(false);
	SetPlayerFrozen(false);
	ApplyInspectionInputMode(false);
	SetPlayerVisualsHidden(false);
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

void UInspectComponent::SetPlayerVisualsHidden(bool bHidden)
{
	ACharacter* Character = GetOwnerCharacter();
	if (!Character) { return; }
	
	if (USkeletalMeshComponent* Body = Character->GetMesh())
	{
		Body->SetOwnerNoSee(bHidden);
	}
	
	if (const APlayerCharacter* Player = Cast<APlayerCharacter>(Character))
	{
		if (UCombatComponent* Combat = Player->GetCombatComponent())
		{
			if (AWeapon* Weapon = Combat->GetCurrentWeapon())
			{
				if (USkeletalMeshComponent* WeaponMesh = Weapon->GetWeaponMesh())
				{
					WeaponMesh->SetOwnerNoSee(bHidden);
				}
			}
		}
	}
}
