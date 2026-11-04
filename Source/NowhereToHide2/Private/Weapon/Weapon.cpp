#include "Weapon/Weapon.h"

#include "Camera/CameraComponent.h"
#include "Combat/CombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"
#include "Weapon/Bullet.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
	WeaponMesh->SetGenerateOverlapEvents(false);
	
	WeaponMesh->SetCastShadow(false);
}

void AWeapon::StartReload()
{
	if (bReloading) { return; }
	if (AmmoInClip >= ClipSize) { return; }
	if (ReserveAmmo <= 0) { return; }

	bReloading = true;
	StopFire();

	PlayReloadMontage();

	if (APlayerCharacter* Char = Cast<APlayerCharacter>(GetOwner()))
	{
		if (UCombatComponent* Combat = Char->GetCombatComponent())
		{
			Combat->OnWeaponReload();
		}
	}

	const float Duration = GetReloadDuration();
	GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AWeapon::FinishReload, Duration, false);

	UE_LOG(LogTemp, Log, TEXT("[Weapon] Reloading for %.2fs (clip %d/%d, reserve %d)"), Duration, AmmoInClip, ClipSize, ReserveAmmo);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	AmmoInClip = ClipSize;
}

void AWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopFire();
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
	Super::EndPlay(EndPlayReason);
}

void AWeapon::FireBullet()
{
	if (!BulletClass || !WeaponMesh) { return; }

	const FVector MuzzleLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);
	
	FRotator SpawnRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);

	if (const APlayerCharacter* Character = Cast<APlayerCharacter>(GetOwner()))
	{
		if (const UCameraComponent* Camera = Character->GetCamera())
		{
			SpawnRotation = Camera->GetComponentRotation();
		}
	}

	const float SpreadDegrees = GetCurrentSpreadDegrees();
	if (SpreadDegrees > 0.f)
	{
		const FVector BaseDirection = SpawnRotation.Vector();
		const FVector ScatteredDirection =
			FMath::VRandCone(BaseDirection, FMath::DegreesToRadians(SpreadDegrees));

		SpawnRotation = ScatteredDirection.Rotation();
	}

	FActorSpawnParameters Params;
	Params.Owner = GetOwner();
	Params.Instigator = Cast<APawn>(GetOwner());
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<ABullet>(BulletClass, MuzzleLocation, SpawnRotation, Params);
}

void AWeapon::FireOnce()
{
	if (!bFiring) { return; }
	if (bReloading) { return; }

	if (AmmoInClip <= 0)
	{
		StopFire();
		return;
	}
	--AmmoInClip;

	FireBullet();

	PlayWeaponFireMontage();
	
	if (APlayerCharacter* Char = Cast<APlayerCharacter>(GetOwner()))
	{
		if (UCombatComponent* Combat = Char->GetCombatComponent())
		{
			Combat->OnWeaponShot();
		}
	}
}

void AWeapon::PlayWeaponFireMontage()
{
	if (!WeaponFireMontage || !WeaponMesh) { return; }

	UAnimInstance* Anim = WeaponMesh->GetAnimInstance();
	if (!Anim)
	{
		UE_LOG(LogTemp, Warning,
				TEXT("[Weapon] %s has no AnimInstance, montage cannot play. Assign an Anim Class to WeaponMesh in BP_Weapon."),
				*GetNameSafe(this));
		return;
	}

	const float Length   = WeaponFireMontage->GetPlayLength();
	const float Interval = FMath::Max(FireInterval, 0.01f);
	const float Rate     = (Length > Interval) ? (Length / Interval) : 1.f;

	Anim->Montage_Play(WeaponFireMontage, Rate);
}

void AWeapon::PlayDryFireSound()
{
	if (!DryFireSound || !WeaponMesh) { return; }
	
	UGameplayStatics::SpawnSoundAttached(
		DryFireSound,
		WeaponMesh,
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		false,
		1.f,
		FMath::FRandRange(0.95f, 1.05f));
}

float AWeapon::GetCurrentSpreadDegrees() const
{
	const APlayerCharacter* Char = Cast<APlayerCharacter>(GetOwner());
	if (Char && Char->IsAiming())
	{
		return AimSpreadDegrees;
	}
	return HipSpreadDegrees;
}

void AWeapon::PlayReloadMontage()
{
	UAnimMontage* Montage = SelectReloadMontage();
	if (!Montage) { return; }

	UAnimInstance* Anim = WeaponMesh ? WeaponMesh->GetAnimInstance() : nullptr;
	if (!Anim)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Weapon] %s has no AnimInstance, reload montage cannot play"),
			*GetNameSafe(this));
		return;
	}

	Anim->Montage_Play(Montage, 1.f);
}

UAnimMontage* AWeapon::SelectReloadMontage() const
{
	if (AmmoInClip <= 0 && ReloadEmptyMontage)
	{
		return ReloadEmptyMontage;
	}
	return ReloadMontage;
}

void AWeapon::FinishReload()
{
	if (!bReloading) { return; }
	bReloading = false;

	const int32 Needed = ClipSize - AmmoInClip;
	const int32 Taken  = FMath::Min(Needed, ReserveAmmo);

	AmmoInClip  += Taken;
	ReserveAmmo -= Taken;

	UE_LOG(LogTemp, Log, TEXT("[Weapon] Reload finished. Clip=%d/%d Reserve=%d"), AmmoInClip, ClipSize, ReserveAmmo);
}

float AWeapon::GetReloadDuration() const
{
	float Duration = ReloadFallbackDuration;

	if (const UAnimMontage* Montage = SelectReloadMontage())
	{
		Duration = Montage->GetPlayLength();
	}
	
	if (const APlayerCharacter* Char = Cast<APlayerCharacter>(GetOwner()))
	{
		if (const UCombatComponent* Combat = Char->GetCombatComponent())
		{
			Duration = FMath::Max(Duration, Combat->GetReloadMontageDuration());
		}
	}

	return FMath::Max(Duration, 0.1f);
}

void AWeapon::AttachToMesh(USkeletalMeshComponent* ParentMesh, FName SocketName)
{
	if (!ParentMesh) { return; }

	AttachToComponent(ParentMesh,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		SocketName);
}

void AWeapon::StartFire()
{
	if (bFiring) { return; }
	if (bReloading) { return; }

	if (AmmoInClip <= 0)
	{
		PlayDryFireSound();
		return;
	}

	bFiring = true;

	FireOnce();

	const float Interval = FMath::Max(FireInterval, 0.01f);
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AWeapon::FireOnce, Interval, true);
}

void AWeapon::StopFire()
{
	if (!bFiring) { return; }

	bFiring = false;
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
}
