#include "Interaction/InteractableActor.h"
#include "Components/StaticMeshComponent.h"

AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	
	Mesh->SetCollisionProfileName(TEXT("BlockAll"));
}

bool AInteractableActor::CanInteract_Implementation(APawn* Interactor) const
{
	return bInteractable && Interactor != nullptr;
}

void AInteractableActor::Interact_Implementation(APawn* Interactor)
{
	if (!Interactor) { return; }

	OnInteracted(Interactor);

	if (bOneShot)
	{
		SetInteractable(false);
	}
}

FText AInteractableActor::GetInteractionPrompt_Implementation() const
{
	return PromptText;
}

void AInteractableActor::SetInteractable(bool bNewInteractable)
{
	bInteractable = bNewInteractable;
}