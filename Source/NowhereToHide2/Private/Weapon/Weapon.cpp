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
	
	if (APlayerCharacter* Char = Cast<APlayerCharacter>(GetOwner()))
	{
		if (UCombatComponent* Combat = Char->GetCombatComponent())
		{
			Combat->OnWeaponShot();
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("[Weapon] Bang"));
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
