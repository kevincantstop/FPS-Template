#include "Weapon/Bullet.h"

#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "Enemy/EnemyCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/BulletFXData.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	SetRootComponent(CollisionSphere);

	TracerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TracerMesh"));
	TracerMesh->SetupAttachment(CollisionSphere);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionSphere;
	
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(MaxLifeSpan);

	if (AActor* Shooter = GetOwner())
	{
		CollisionSphere->IgnoreActorWhenMoving(Shooter, true);
	}

	ProjectileMovement->OnProjectileStop.AddDynamic(this, &ABullet::OnProjectileStopped);
}

void ABullet::OnProjectileStopped(const FHitResult& ImpactResult)
{
	ApplyImpactImpulse(ImpactResult);

	const bool bHitFlesh = ImpactResult.GetActor() && ImpactResult.GetActor()->IsA<APawn>();
	const FName DesiredRow = bHitFlesh ? FleshImpactRow : DefaultImpactRow;
	
	const FBulletFXEntry* Entry = ImpactFX ? ImpactFX->FindEntry(DesiredRow) : nullptr;
	if (!Entry && ImpactFX && DesiredRow != DefaultImpactRow)
	{
		Entry = ImpactFX->FindEntry(DefaultImpactRow);
	}
	if (Entry)
	{
		SpawnImpactVFX(ImpactResult, Entry->ImpactVFX);
		SpawnImpactDecal(ImpactResult, Entry->DecalMaterial);
		PlayImpactSound(ImpactResult, Entry->ImpactSound);
	}
	if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(ImpactResult.GetActor()))
	{
		Enemy->PlayHitReaction(ImpactResult);
	}

	Destroy();
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABullet::SpawnImpactDecal(const FHitResult& Impact, UMaterialInterface* InDecalMaterial)
{
	if (!InDecalMaterial) { return; }

	const FRotator DecalRotation = (-Impact.ImpactNormal).Rotation();

	UDecalComponent* Decal;

	if (USceneComponent* HitComponent = Impact.GetComponent())
	{
		Decal = UGameplayStatics::SpawnDecalAttached(
			InDecalMaterial, DecalSize, HitComponent, Impact.BoneName,
			Impact.ImpactPoint, DecalRotation,
			EAttachLocation::KeepWorldPosition, DecalLifeSpan);
	}
	else
	{
		Decal = UGameplayStatics::SpawnDecalAtLocation(
			this, InDecalMaterial, DecalSize, Impact.ImpactPoint,
			DecalRotation, DecalLifeSpan);
	}

	if (Decal)
	{
		Decal->SetFadeScreenSize(0.f);
	}
}

void ABullet::PlayImpactSound(const FHitResult& Impact, USoundBase* InSound)
{
	if (!InSound) { return; }

	UGameplayStatics::PlaySoundAtLocation(
		this, InSound, Impact.Location, FRotator::ZeroRotator,
		1.f, FMath::FRandRange(0.95f, 1.05f));
}

void ABullet::SpawnImpactVFX(const FHitResult& Impact, UParticleSystem* InVFX)
{
	if (!InVFX) { return; }

	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(), InVFX, Impact.Location, Impact.ImpactNormal.Rotation(),
		true, EPSCPoolMethod::AutoRelease);
}

void ABullet::ApplyImpactImpulse(const FHitResult& Impact)
{
	UPrimitiveComponent* HitComponent = Impact.GetComponent();
	if (!HitComponent) { return; }
	
	if (!HitComponent->IsSimulatingPhysics(Impact.BoneName)) { return; }
	
	const FVector Direction = -Impact.ImpactNormal;
	const FVector Impulse = Direction * ProjectileMovement->InitialSpeed * ImpactImpulse;

	HitComponent->AddImpulseAtLocation(Impulse, Impact.Location, Impact.BoneName);
}
