#include "Interaction/Door/InteractiveDoor.h"

#include "Components/StaticMeshComponent.h"

AInteractiveDoor::AInteractiveDoor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	bOneShot = false;
	
	Hinge = CreateDefaultSubobject<USceneComponent>(TEXT("Hinge"));
	Hinge->SetupAttachment(Mesh);
	
	DoorPanel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorPanel"));
	DoorPanel->SetupAttachment(Hinge);
	DoorPanel->SetCollisionProfileName(TEXT("BlockAll"));

	OpenPrompt  = FText::FromString(TEXT("[E] Open door"));
	ClosePrompt = FText::FromString(TEXT("[E] Close door"));
}

void AInteractiveDoor::BeginPlay()
{
	Super::BeginPlay();

	ClosedHingeRotation = Hinge ? Hinge->GetRelativeRotation() : FRotator::ZeroRotator;
}

bool AInteractiveDoor::CanInteract_Implementation(APawn* Interactor) const
{
	if (!Super::CanInteract_Implementation(Interactor)) { return false; }

	if (bAnimating) { return false; }
	if (bOpen && !bCanClose) { return false; }

	return true;
}

FText AInteractiveDoor::GetInteractionPrompt_Implementation() const
{
	return bOpen ? ClosePrompt : OpenPrompt;
}

void AInteractiveDoor::Interact_Implementation(APawn* Interactor)
{
	if (bAnimating) { return; }
	if (bOpen && !bCanClose) { return; }

	Super::Interact_Implementation(Interactor);

	StartSwing(!bOpen);
}

void AInteractiveDoor::StartSwing(bool bNewOpen)
{
	if (!Hinge) { return; }

	bOpen = bNewOpen;
	bAnimating = true;

	StartYaw  = Hinge->GetRelativeRotation().Yaw;
	TargetYaw = ClosedHingeRotation.Yaw + (bOpen ? OpenAngle : 0.f);
	AnimElapsed = 0.f;

	if (OpenDuration <= KINDA_SMALL_NUMBER)
	{
		FinishSwing();
		return;
	}

	SetActorTickEnabled(true);
}

void AInteractiveDoor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!Hinge) { return; }

	AnimElapsed += DeltaSeconds;

	const float Alpha  = FMath::Clamp(AnimElapsed / OpenDuration, 0.f, 1.f);
	const float Eased  = FMath::InterpEaseInOut(0.f, 1.f, Alpha, 2.f);

	FRotator Rot = ClosedHingeRotation;
	Rot.Yaw = FMath::Lerp(StartYaw, TargetYaw, Eased);
	Hinge->SetRelativeRotation(Rot);

	if (Alpha >= 1.f)
	{
		FinishSwing();
	}
}

void AInteractiveDoor::FinishSwing()
{
	bAnimating = false;
	SetActorTickEnabled(false);
}