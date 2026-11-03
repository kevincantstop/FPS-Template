#include "Weapon/Weapon.h"

#include "Combat/CombatComponent.h"
#include "Player/PlayerCharacter.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
	WeaponMesh->SetGenerateOverlapEvents(false);
	
	WeaponMesh->SetCastShadow(false);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopFire();
	Super::EndPlay(EndPlayReason);
}

void AWeapon::FireOnce()
{
	if (!bFiring) { return; }

	PlayWeaponFireMontage();
	
	if (APlayerCharacter* Char = Cast<APlayerCharacter>(GetOwner()))
	{
		if (UCombatComponent* Combat = Char->GetCombatComponent())
		{
			Combat->OnWeaponShot();
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("[Weapon] Bang"));
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
