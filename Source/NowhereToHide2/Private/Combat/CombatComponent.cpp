#include "Combat/CombatComponent.h"

#include "Camera/CameraComponent.h"
#include "Player/PlayerCharacter.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<APlayerCharacter>(GetOwner());
	Camera = OwnerCharacter ? OwnerCharacter->GetCamera() : nullptr;
	
	if (Camera)
	{
		Camera->SetFieldOfView(DefaultFOV);
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	UpdateCameraFOV(DeltaTime);
}

void UCombatComponent::UpdateCameraFOV(float DeltaTime)
{
	if (!Camera || !OwnerCharacter) { return; }

	const float TargetFOV = OwnerCharacter->IsAiming() ? AimFOV : DefaultFOV;
	Camera->SetFieldOfView(FMath::FInterpTo(Camera->FieldOfView, TargetFOV, DeltaTime, FOVInterpSpeed));
}

