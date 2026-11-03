#include "Weapon/Weapon.h"

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

void AWeapon::AttachToMesh(USkeletalMeshComponent* ParentMesh, FName SocketName)
{
	if (!ParentMesh) { return; }

	AttachToComponent(ParentMesh,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		SocketName);
}