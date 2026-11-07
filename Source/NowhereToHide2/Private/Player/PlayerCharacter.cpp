// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Combat/CombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/RectLightComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Health/HealthComponent.h"
#include "Kismet/GameplayStatics.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetMesh());
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bEnableCameraLag = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;

	RectLight = CreateDefaultSubobject<URectLightComponent>(TEXT("RectLight"));
	RectLight->SetupAttachment(GetCapsuleComponent());

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	HitPostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("HitPostProcess"));
	HitPostProcess->SetupAttachment(Camera);
	HitPostProcess->bUnbound = true;
	HitPostProcess->Priority = 10.f;
	HitPostProcess->BlendWeight = 0.f;

	DeathPostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("DeathPostProcess"));
	DeathPostProcess->SetupAttachment(Camera);
	DeathPostProcess->bUnbound = true;
	DeathPostProcess->Priority = 20.f;
	DeathPostProcess->BlendWeight = 0.f;

	GetMesh()->CastShadow = false;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	ApplyMovementSpeed();

	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddDynamic(this, &APlayerCharacter::HandleHealthChanged);
		HealthComponent->OnDeath.AddDynamic(this, &APlayerCharacter::HandleDeath);
	}
}

void APlayerCharacter::HandleDeath()
{
	if (DeathSound)
	{
		UGameplayStatics::SpawnSound2D(this, DeathSound);
	}
}

void APlayerCharacter::HandleHealthChanged(float NewHealth, float DeltaHealth)
{
	if (DeltaHealth >= 0.f) { return; }

	if (HitSound && GetMesh())
	{
		UGameplayStatics::SpawnSoundAttached(HitSound, GetMesh());
	}
	if (HitCameraShake)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			PC->ClientStartCameraShake(HitCameraShake, 5);
		}
	}
	if (HitPostProcess)
	{
		HitPostProcess->BlendWeight = FMath::Max(HitPostProcess->BlendWeight, HitPostProcessWeight);
	}
}

bool APlayerCharacter::CanJumpInternal_Implementation() const
{
	return bJumpEnabled && Super::CanJumpInternal_Implementation();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateMovementSpeed(DeltaTime);
	UpdatePostProcess(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerCharacter::SetSprinting(bool bNewSprinting)
{
	if (bIsSprinting == bNewSprinting) { return; }

	bIsSprinting = bNewSprinting;
}

void APlayerCharacter::SetJumpEnabled(bool bEnabled)
{
	if (bJumpEnabled == bEnabled) { return; }

	bJumpEnabled = bEnabled;
	
	if (!bJumpEnabled)
	{
		StopJumping();
	}
}

void APlayerCharacter::SetAiming(bool bNewAiming)
{
	if (bIsAiming == bNewAiming) { return; }
	bIsAiming = bNewAiming;

	if (bIsAiming)
	{
		SetSprinting(false);
	}
}

void APlayerCharacter::StartFire()
{
	if (bIsSprinting)
	{
		SetSprinting(false);
	}
	if (CombatComponent)
	{
		CombatComponent->StartFire();
	}
}

void APlayerCharacter::StopFire()
{
	if (CombatComponent) { CombatComponent->StopFire(); }
}

void APlayerCharacter::Reload()
{
	if (CombatComponent)
	{
		CombatComponent->Reload();
	}
}

void APlayerCharacter::ApplyMovementSpeed()
{
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = bIsSprinting ? SprintSpeed : WalkSpeed;
	}
}

void APlayerCharacter::UpdateMovementSpeed(float DeltaTime)
{
	UCharacterMovementComponent* Move = GetCharacterMovement();
	if (!Move) { return; }

	const float TargetSpeed = bIsSprinting ? SprintSpeed : WalkSpeed;
	
	Move->MaxWalkSpeed = FMath::FInterpTo(Move->MaxWalkSpeed, TargetSpeed, DeltaTime, SprintInterpSpeed);
}

void APlayerCharacter::UpdatePostProcess(float DeltaTime)
{
	if (!HitPostProcess || !DeathPostProcess) { return; }
	
	if (bDebugShowHitPostProcess)
	{
		HitPostProcess->BlendWeight = 1.f;
	}
	else
	{
		const bool bDead = HealthComponent && HealthComponent->IsDead();
		const float HitFadeSpeed = bDead ? 6.f : HitPostProcessFadeSpeed;

		HitPostProcess->BlendWeight = FMath::FInterpTo(
			HitPostProcess->BlendWeight, 0.f, DeltaTime, HitFadeSpeed);
	}
	
	if (bDebugShowDeathPostProcess)
	{
		DeathPostProcess->BlendWeight = 1.f;
	}
	else
	{
		const bool bDead = HealthComponent && HealthComponent->IsDead();

		DeathPostProcess->BlendWeight = FMath::FInterpTo(
			DeathPostProcess->BlendWeight, bDead ? 1.f : 0.f, DeltaTime, DeathPostProcessFadeSpeed);
	}
}

