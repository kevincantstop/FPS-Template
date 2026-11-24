#include "Interaction/Inspect/InspectItem.h"

#include "Components/StaticMeshComponent.h"
#include "Interaction/Inspect/InspectComponent.h"
#include "Player/PlayerCharacter.h"

AInspectItem::AInspectItem()
{
	bOneShot = false;
	PromptText = FText::FromString(TEXT("[E] Inspect"));

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bTickEvenWhenPaused = true;

	InspectionPivot = CreateDefaultSubobject<USceneComponent>(TEXT("InspectionPivot"));
	InspectionPivot->SetupAttachment(Mesh);

	InspectionRoot = CreateDefaultSubobject<USceneComponent>(TEXT("InspectionRoot"));
	InspectionRoot->SetupAttachment(InspectionPivot);
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(InspectionRoot);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ItemMesh->SetCastShadow(false);
}

void AInspectItem::BeginPlay()
{
	Super::BeginPlay();
	
	if (Mesh)
	{
		if (!Mesh->GetStaticMesh() && ItemMesh)
		{
			Mesh->SetStaticMesh(ItemMesh->GetStaticMesh());
		}
		Mesh->SetHiddenInGame(true);
		Mesh->SetMobility(EComponentMobility::Movable);
	}

	PivotBaseRotation    = InspectionPivot ? InspectionPivot->GetRelativeRotation() : FRotator::ZeroRotator;
	CurrentPivotRotation = PivotBaseRotation;
}

void AInspectItem::Interact_Implementation(APawn* Interactor)
{
	if (bInspecting || bMoving) { return; }

	APlayerCharacter* Player = Cast<APlayerCharacter>(Interactor);
	if (!Player || !Player->GetInspectComponent()) { return; }

	Super::Interact_Implementation(Interactor);

	Player->GetInspectComponent()->BeginInspect(this);
}

void AInspectItem::BeginInspect(const FTransform& TargetTransform)
{
	if (bInspecting) { return; }

	bInspecting = true;
	
	OriginalTransform = GetActorTransform();

	if (InspectionPivot)
	{
		PivotBaseRotation    = InspectionPivot->GetRelativeRotation();
		CurrentPivotRotation = PivotBaseRotation;
		InspectionPivot->SetRelativeRotation(CurrentPivotRotation);
	}
	
	SetActorEnableCollision(false);
	
	if (InspectionRoot)
	{
		InspectionRoot->SetRelativeScale3D(FVector::OneVector);
	}
	
	FTransform FinalTarget = TargetTransform;

	const FRotator TargetFacing = (TargetTransform.GetRotation().Rotator() + InspectFacingOffset).GetNormalized();
	FinalTarget.SetRotation(TargetFacing.Quaternion());

	StartMove(FinalTarget, false);
}

void AInspectItem::EndInspect()
{
	
}

float AInspectItem::BeginReturn()
{
	if (!bInspecting) { return 0.f; }

	StartMove(OriginalTransform, true);
	return MoveDuration;
}

void AInspectItem::StartMove(const FTransform& Target, bool bInReturning)
{
	MoveTarget  = Target;
	MoveElapsed = 0.f;
	bMoving     = true;
	bReturning  = bInReturning;

	SetActorTickEnabled(true);
}

void AInspectItem::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bMoving) { return; }

	MoveElapsed += DeltaSeconds;

	const float Alpha = FMath::Clamp(MoveElapsed / FMath::Max(MoveDuration, 0.01f), 0.f, 1.f);
	const float Eased = FMath::InterpEaseInOut(0.f, 1.f, Alpha, 2.f);
	
	const FTransform Current = GetActorTransform();

	FTransform Next;
	Next.Blend(Current, MoveTarget, Eased);

	SetActorTransform(Next, false, nullptr, ETeleportType::TeleportPhysics);
	
	if (InspectionPivot)
	{
		InspectionPivot->SetRelativeRotation(
			FMath::Lerp(PivotBaseRotation, CurrentPivotRotation, Eased));
	}
	
	if (InspectionRoot)
	{
		const FVector From = bReturning ? DesiredInspectScale : FVector::OneVector;
		const FVector To   = bReturning ? FVector::OneVector : DesiredInspectScale;

		InspectionRoot->SetRelativeScale3D(FMath::Lerp(From, To, Eased));
	}

	if (Alpha >= 1.f)
	{
		bMoving = false;
		SetActorTickEnabled(false);

		if (bReturning)
		{
			bInspecting = false;
			SetActorEnableCollision(true);
		}
	}
}

void AInspectItem::AddInspectRotation(const FVector2D& DeltaDegrees)
{
	if (!bAllowRotation || bMoving || !InspectionPivot) { return; }
	
	CurrentPivotRotation.Yaw   += DeltaDegrees.X;
	CurrentPivotRotation.Pitch  = FMath::Clamp(
		CurrentPivotRotation.Pitch + DeltaDegrees.Y, -MaxPitch, MaxPitch);

	InspectionPivot->SetRelativeRotation(CurrentPivotRotation);
}