#include "Enemy/EnemyCharacter.h"

#include "Enemy/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

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
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

void AEnemyCharacter::PlayHitReaction(const FHitResult& Impact)
{
	USkeletalMeshComponent* SkelMesh = GetMesh();
	if (!SkelMesh) { return; }

	UAnimInstance* Anim = SkelMesh->GetAnimInstance();
	if (!Anim) { return; }
	
	if (Anim->IsAnyMontagePlaying()) { return; }
	
	FName BoneName = Impact.BoneName;
	if (BoneName.IsNone())
	{
		BoneName = SkelMesh->FindClosestBone(Impact.ImpactPoint);
	}

	UAnimMontage* Montage = SelectHitMontage(BoneName);
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
