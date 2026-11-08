#include "UI/MainMenuPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MainMenuWidget.h"

void AMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UseRandomCamera();

	if (!MainMenuWidgetClass) { return; }

	MainMenuWidget = CreateWidget<UMainMenuWidget>(this, MainMenuWidgetClass);
	if (!MainMenuWidget) { return; }

	MainMenuWidget->AddToViewport();

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);

	bShowMouseCursor = true;

	if (bRotateCameras)
	{
		GetWorldTimerManager().SetTimer(
			CameraRotateTimer, this, &AMainMenuPlayerController::UseRandomCamera,
			CameraRotateInterval, true);
	}
}

void AMainMenuPlayerController::UseRandomCamera()
{
	UWorld* World = GetWorld();
	if (!World) { return; }

	TArray<AActor*> FoundCameras;
	UGameplayStatics::GetAllActorsOfClass(World, ACameraActor::StaticClass(), FoundCameras);

	if (FoundCameras.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[MainMenu] No ACameraActor found in level"));
		return;
	}

	AActor* Chosen = FoundCameras[FMath::RandRange(0, FoundCameras.Num() - 1)];
	
	SetViewTargetWithBlend(Chosen, bRotateCameras ? 2.f : 0.f);
}