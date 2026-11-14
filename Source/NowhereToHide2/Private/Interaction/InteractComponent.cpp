#include "Interaction/InteractComponent.h"

#include "Interaction/Interactable.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

UInteractComponent::UInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
}

void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	SetFocusedActor(nullptr);
}

void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeSinceLastCheck += DeltaTime;
	if (TimeSinceLastCheck < UpdateInterval) { return; }
	TimeSinceLastCheck = 0.f;

	UpdateInteractionTarget();
}

void UInteractComponent::UpdateInteractionTarget()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	UWorld* World = GetWorld();
	if (!OwnerPawn || !World) { return; }
	
	APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController());
	if (!PC || !PC->IsLocalController())
	{
		SetFocusedActor(nullptr);
		return;
	}
	
	FVector ViewLocation;
	FRotator ViewRotation;
	PC->GetPlayerViewPoint(ViewLocation, ViewRotation);

	const FVector End = ViewLocation + ViewRotation.Vector() * InteractionRange;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(InteractionTrace), false, GetOwner());
	Params.AddIgnoredActor(GetOwner());

	FHitResult Hit;
	const bool bHit = World->LineTraceSingleByChannel(
		Hit, ViewLocation, End, ECC_Visibility, Params);

	AActor* NewTarget = nullptr;

	if (bHit)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor
			&& HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass())
			&& IInteractable::Execute_CanInteract(HitActor, OwnerPawn))
		{
			NewTarget = HitActor;
		}
	}

	SetFocusedActor(NewTarget);
}

void UInteractComponent::SetFocusedActor(AActor* NewActor)
{
	if (FocusedActor == NewActor) { return; }

	FocusedActor = NewActor;

	InteractionPrompt = FocusedActor
		? IInteractable::Execute_GetInteractionPrompt(FocusedActor)
		: FText::GetEmpty();

	OnInteractionTargetChanged.Broadcast();
}

bool UInteractComponent::TryInteract()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!FocusedActor || !OwnerPawn) { return false; }

	if (!IInteractable::Execute_CanInteract(FocusedActor, OwnerPawn)) { return false; }

	IInteractable::Execute_Interact(FocusedActor, OwnerPawn);
	return true;
}