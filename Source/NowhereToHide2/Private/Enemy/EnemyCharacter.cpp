#include "Enemy/EnemyCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Enemy/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Health/HealthComponent.h"

static bool BoneMatchesAnyKeyword(const FString& BoneLower, const TArray<FString>& Keywords)
{
	for (const FString& Keyword : Keywords)
	{
		const FString Trimmed = Keyword.TrimStartAndEnd().ToLower();
		if (!Trimmed.IsEmpty() && BoneLower.Contains(Trimmed))
		{
			return true;
		}
	}
	return false;
}

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI    = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	
	GetCharacterMovement()->bUseRVOAvoidance = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComponent->OnDeath.AddDynamic(this, &AEnemyCharacter::HandleDeath);
	
	ApplyRandomMesh();
	
	bCanSprint = FMath::FRand() < SprintChance;
	
	EffectiveRunSpeed = RunSpeed + FMath::FRandRange(-SpeedVariation, SpeedVariation);

	SetSprinting(bCanSprint);
	
	ApplyMovementSpeed();
}

void AEnemyCharacter::SetSprinting(bool bNewSprinting)
{
	if (!bCanSprint)
	{
		bNewSprinting = false;
	}

	if (bIsSprinting == bNewSprinting) { return; }

	bIsSprinting = bNewSprinting;
	ApplyMovementSpeed();
}

void AEnemyCharacter::ApplyMovementSpeed()
{
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = bIsSprinting ? SprintSpeed : EffectiveRunSpeed;
	}
}

void AEnemyCharacter::ApplyRandomMesh()
{
	if (MeshPool.Num() == 0) { return; }

	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp) { return; }

	USkeletalMesh* Picked = MeshPool[FMath::RandRange(0, MeshPool.Num() - 1)];
	if (!Picked) { return; }

	MeshComp->SetSkeletalMeshAsset(Picked);
}

void AEnemyCharacter::PlayHitReaction(FName ResolvedBone)
{
	USkeletalMeshComponent* SkelMesh = GetMesh();
	if (!SkelMesh) { return; }

	UAnimInstance* Anim = SkelMesh->GetAnimInstance();
	if (!Anim) { return; }
	
	if (Anim->IsAnyMontagePlaying()) { return; }

	UAnimMontage* Montage = SelectHitMontage(ResolvedBone);
	if (!Montage) { return; }

	Anim->Montage_Play(Montage, 1.f);
}

UAnimMontage* AEnemyCharacter::SelectHitMontage(FName BoneName) const
{
	const FString Bone = BoneName.ToString().ToLower();

	if (BoneMatchesAnyKeyword(Bone, HeadBoneKeywords))     { return HitHeadMontage; }
	if (BoneMatchesAnyKeyword(Bone, LeftArmBoneKeywords))  { return HitLeftArmMontage; }
	if (BoneMatchesAnyKeyword(Bone, RightArmBoneKeywords)) { return HitRightArmMontage; }

	return nullptr;
}

FName AEnemyCharacter::ResolveHitBone(const FHitResult& Impact) const
{
	if (!Impact.BoneName.IsNone())
	{
		return Impact.BoneName;
	}
	const USkeletalMeshComponent* SkelMesh = GetMesh();
	return SkelMesh ? SkelMesh->FindClosestBone(Impact.ImpactPoint) : NAME_None;
}

void AEnemyCharacter::ReceiveBulletHit(const FHitResult& Impact, float BaseDamage)
{
	const FName BoneName = ResolveHitBone(Impact);
	const bool bHeadshot = BoneMatchesAnyKeyword(BoneName.ToString().ToLower(), HeadBoneKeywords);

	PlayHitReaction(BoneName);

	const float FinalDamage = BaseDamage * (bHeadshot ? HeadshotMultiplier : 1.f);

	if (HealthComponent)
	{
		HealthComponent->ApplyDamage(FinalDamage);
	}

	UE_LOG(LogTemp, Log, TEXT("[Enemy] bone=%s damage=%.1f%s"),
		*BoneName.ToString(), FinalDamage, bHeadshot ? TEXT(" (HEADSHOT)") : TEXT(""));
}

void AEnemyCharacter::HandleDeath()
{
	if (AAIController* AI = Cast<AAIController>(GetController()))
	{
		AI->StopMovement();
	}
	
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	UAnimInstance* Anim = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	
	if (DeathMontage && Anim)
	{
		Anim->Montage_Play(DeathMontage, 1.f);
		
		FTimerHandle DeathAnimTimer;
		GetWorldTimerManager().SetTimer(
			DeathAnimTimer, this, &AEnemyCharacter::EnterRagdoll,
			DeathMontage->GetPlayLength() * 0.8f, false);
	}
	else
	{
		EnterRagdoll();
	}
}

void AEnemyCharacter::EnterRagdoll()
{
	USkeletalMeshComponent* SkelMesh = GetMesh();
	if (!SkelMesh) { return; }

	SkelMesh->SetCollisionProfileName(TEXT("Ragdoll"));
	SkelMesh->SetAllBodiesSimulatePhysics(true);
	SkelMesh->WakeAllRigidBodies();
	SkelMesh->bBlendPhysics = true;
}
