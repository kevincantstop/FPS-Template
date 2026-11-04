#include "Weapon/Bullet.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	SetRootComponent(CollisionSphere);

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
	
	SpawnImpactVFX(ImpactResult);
	SpawnImpactDecal(ImpactResult);
	PlayImpactSound(ImpactResult);
	
	Destroy();
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABullet::SpawnImpactDecal(const FHitResult& Impact)
{
	if (!DecalMaterial) { return; }
	
	const FRotator DecalRotation = (-Impact.ImpactNormal).Rotation();
	const FVector DecalSize = FVector(7.f, 7.f, 7.f);
	constexpr float DecalLifeSpan = 10.f;

	if (USceneComponent* HitComponent = Impact.GetComponent())
	{
		UGameplayStatics::SpawnDecalAttached(
			DecalMaterial,
			DecalSize,
			HitComponent,
			Impact.BoneName,
			Impact.Location,
			DecalRotation,
			EAttachLocation::KeepWorldPosition,
			DecalLifeSpan);
		return;
	}
	
	UGameplayStatics::SpawnDecalAtLocation(this, DecalMaterial, DecalSize, Impact.Location, DecalRotation, DecalLifeSpan);
}

void ABullet::PlayImpactSound(const FHitResult& Impact)
{
	if (!ImpactSound) { return; }

	UGameplayStatics::PlaySoundAtLocation(
		this,
		ImpactSound,
		Impact.Location,
		FRotator::ZeroRotator,
		1.f,
		FMath::FRandRange(0.95f, 1.05f));
}

void ABullet::SpawnImpactVFX(const FHitResult& Impact)
{
	if (!ImpactVFX) { return; }

	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		ImpactVFX,
		Impact.Location,
		Impact.ImpactNormal.Rotation(),
		true,
		EPSCPoolMethod::AutoRelease);
}

void ABullet::ApplyImpactImpulse(const FHitResult& Impact)
{
	UPrimitiveComponent* HitComponent = Impact.GetComponent();
	if (!HitComponent) { return; }
	
	if (!HitComponent->IsSimulatingPhysics(Impact.BoneName)) { return; }
	
	const FVector Direction = -Impact.ImpactNormal;
	
	const float Mass = HitComponent->GetMass();
	const FVector Impulse = Direction * ProjectileMovement->InitialSpeed * FMath::Max(Mass, 0.1f);

	HitComponent->AddImpulseAtLocation(Impulse, Impact.Location, Impact.BoneName);
}
