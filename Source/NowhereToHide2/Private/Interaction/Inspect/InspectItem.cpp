#include "Interaction/Inspect/InspectItem.h"

#include "Interaction/Inspect/InspectComponent.h"
#include "Player/PlayerCharacter.h"

AInspectItem::AInspectItem()
{
	bOneShot = false;
	PromptText = FText::FromString(TEXT("[E] Inspect"));
}

void AInspectItem::Interact_Implementation(APawn* Interactor)
{
	if (bInspecting) { return; }

	APlayerCharacter* Player = Cast<APlayerCharacter>(Interactor);
	if (!Player || !Player->GetInspectComponent()) { return; }

	Super::Interact_Implementation(Interactor);

	Player->GetInspectComponent()->BeginInspect(this);
}

void AInspectItem::BeginInspect(const FTransform& TargetTransform)
{
	OriginalTransform = GetActorTransform();
	
	SetActorEnableCollision(false);

	SetActorTransform(TargetTransform, false, nullptr, ETeleportType::TeleportPhysics);

	CurrentRotation = InspectRotation;
	SetActorRotation(CurrentRotation);

	bInspecting = true;
}

void AInspectItem::EndInspect()
{
	if (!bInspecting) { return; }

	bInspecting = false;

	SetActorTransform(OriginalTransform, false, nullptr, ETeleportType::TeleportPhysics);
	SetActorEnableCollision(true);

	ResetInspectRotation();
}

void AInspectItem::AddInspectRotation(const FVector2D& DeltaDegrees)
{
	if (!bAllowRotation) { return; }

	CurrentRotation.Yaw   += DeltaDegrees.X;
	CurrentRotation.Pitch  = FMath::Clamp(CurrentRotation.Pitch + DeltaDegrees.Y, -80.f, 80.f);

	SetActorRotation(CurrentRotation);
}

void AInspectItem::ResetInspectRotation()
{
	CurrentRotation = InspectRotation;

	if (bInspecting)
	{
		SetActorRotation(CurrentRotation);
	}
}