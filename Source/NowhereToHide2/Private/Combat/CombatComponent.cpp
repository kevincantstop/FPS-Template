#include "Combat/CombatComponent.h"

#include "Camera/CameraComponent.h"
#include "Player/PlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"

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

void UCombatComponent::StartFire()
{
	bIsFiring = true;
	PlayFireMontage();
}

void UCombatComponent::StopFire()
{
	bIsFiring = false;
}

void UCombatComponent::PlayFireMontage()
{
	if (!OwnerCharacter) { return; }

	UAnimMontage* Montage = SelectFireMontage();
	if (!Montage) { return; }

	USkeletalMeshComponent* SkelMesh = OwnerCharacter->GetMesh();
	UAnimInstance* Anim = SkelMesh ? SkelMesh->GetAnimInstance() : nullptr;
	if (!Anim) { return; }
	
	if (Anim->Montage_IsPlaying(FireMontage) || Anim->Montage_IsPlaying(AimFireMontage))
	{
		return;
	}

	if (Anim->Montage_Play(Montage, 1.f) <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire montage failed to play: %s"), *GetNameSafe(Montage));
	}
}

UAnimMontage* UCombatComponent::SelectFireMontage() const
{
	if (OwnerCharacter && OwnerCharacter->IsAiming() && AimFireMontage)
	{
		return AimFireMontage;
	}
	return FireMontage;
}

