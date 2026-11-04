#include "Combat/CombatComponent.h"

#include "Camera/CameraComponent.h"
#include "Player/PlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "Weapon/Weapon.h"

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

	SpawnDefaultWeapon();
}

void UCombatComponent::SpawnDefaultWeapon()
{
	if (!DefaultWeaponClass || !OwnerCharacter) { return; }

	FActorSpawnParameters Params;
	Params.Owner = OwnerCharacter;
	Params.Instigator = OwnerCharacter;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(
		DefaultWeaponClass, OwnerCharacter->GetActorTransform(), Params);

	if (!Weapon) { return; }

	EquipWeapon(Weapon);
}

void UCombatComponent::EquipWeapon(AWeapon* NewWeapon)
{
	if (!NewWeapon || !OwnerCharacter) { return; }

	USkeletalMeshComponent* ArmsMesh = OwnerCharacter->GetMesh();
	if (!ArmsMesh) { return; }
	
	if (CurrentWeapon && CurrentWeapon != NewWeapon)
	{
		StopFire();
		CurrentWeapon->Destroy();
	}

	NewWeapon->SetOwner(OwnerCharacter);
	NewWeapon->AttachToMesh(ArmsMesh, WeaponSocketName);
	CurrentWeapon = NewWeapon;
}

AWeapon* UCombatComponent::GetCurrentWeapon() const
{
	return CurrentWeapon;
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
	if (bIsFiring) { return; }
	if (!CurrentWeapon) { return; }

	bIsFiring = true;
	CurrentWeapon->StartFire();
}

void UCombatComponent::StopFire()
{
	if (!bIsFiring) { return; }

	bIsFiring = false;

	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

void UCombatComponent::OnWeaponShot()
{
	PlayFireMontage();
}

void UCombatComponent::OnWeaponReload()
{
	PlayReloadMontage();
}

void UCombatComponent::Reload()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartReload();
	}
}

float UCombatComponent::GetReloadMontageDuration() const
{
	const UAnimMontage* Montage = SelectReloadMontage();
	return Montage ? Montage->GetPlayLength() : 0.f;
}

void UCombatComponent::PlayFireMontage()
{
	if (!OwnerCharacter) { return; }

	UAnimMontage* Montage = SelectFireMontage();
	if (!Montage) { return; }

	USkeletalMeshComponent* SkelMesh = OwnerCharacter->GetMesh();
	UAnimInstance* Anim = SkelMesh ? SkelMesh->GetAnimInstance() : nullptr;
	if (!Anim) { return; }
	
	const float Length   = Montage->GetPlayLength();
	const float Interval = FMath::Max(CurrentWeapon ? CurrentWeapon->GetFireInterval() : 0.12f, 0.01f);
	const float Rate     = (Length > Interval) ? (Length / Interval) : 1.f;

	Anim->Montage_Play(Montage, Rate);
}

UAnimMontage* UCombatComponent::SelectFireMontage() const
{
	if (OwnerCharacter && OwnerCharacter->IsAiming() && AimFireMontage)
	{
		return AimFireMontage;
	}
	return FireMontage;
}

void UCombatComponent::PlayReloadMontage()
{
	UAnimMontage* Montage = SelectReloadMontage();
	if (!Montage || !OwnerCharacter) { return; }

	USkeletalMeshComponent* ArmsMesh = OwnerCharacter->GetMesh();
	UAnimInstance* Anim = ArmsMesh ? ArmsMesh->GetAnimInstance() : nullptr;
	if (!Anim) { return; }

	Anim->Montage_Play(Montage, 1.f);
}

UAnimMontage* UCombatComponent::SelectReloadMontage() const
{
	const bool bClipEmpty = CurrentWeapon && CurrentWeapon->GetAmmoInClip() <= 0;

	if (bClipEmpty && ReloadEmptyMontage)
	{
		return ReloadEmptyMontage;
	}
	return ReloadMontage;
}

void UCombatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CurrentWeapon)
	{
		StopFire();
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

