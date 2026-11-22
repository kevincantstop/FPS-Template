#include "Interaction/InspectMeshActor.h"

#include "Components/PointLightComponent.h"
#include "Components/SceneCaptureComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SkyLightComponent.h"

AInspectMeshActor::AInspectMeshActor()
{
	PrimaryActorTick.bCanEverTick = true;

	PreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMesh"));
	SetRootComponent(PreviewMesh);
	PreviewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PreviewMesh->SetCastShadow(false);

	Capture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Capture"));
	Capture->SetupAttachment(PreviewMesh);
	Capture->SetRelativeLocation(FVector(-120.f, 0.f, 0.f));
	Capture->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	
	Capture->bCaptureEveryFrame = false;
	Capture->bCaptureOnMovement  = false;
	Capture->CaptureSource = SCS_FinalColorLDR;
	
	Capture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	Capture->ShowOnlyActors.Add(this);

	KeyLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("KeyLight"));
	KeyLight->SetupAttachment(PreviewMesh);
	KeyLight->SetRelativeLocation(FVector(-100.f, 80.f, 120.f));
	KeyLight->SetIntensity(3000.f);
	KeyLight->SetAttenuationRadius(600.f);

	FillLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("FillLight"));
	FillLight->SetupAttachment(PreviewMesh);
	FillLight->SetIntensity(1.5f);
	FillLight->bLowerHemisphereIsBlack = false;
}

void AInspectMeshActor::SetPreviewMesh(UStaticMesh* InMesh, const FRotator& InitialRotation)
{
	if (!PreviewMesh) { return; }

	PreviewMesh->SetStaticMesh(InMesh);

	BaseRotation = InitialRotation;
	ResetRotation();
}

void AInspectMeshActor::AddPreviewRotation(float DeltaYaw, float DeltaPitch)
{
	CurrentYaw   += DeltaYaw;
	CurrentPitch  = FMath::Clamp(CurrentPitch + DeltaPitch, -80.f, 80.f);

	if (PreviewMesh)
	{
		PreviewMesh->SetRelativeRotation(
			FRotator(CurrentPitch, BaseRotation.Yaw + CurrentYaw, 0.f));
	}
}

void AInspectMeshActor::ResetRotation()
{
	CurrentYaw = 0.f;
	CurrentPitch = 0.f;

	if (PreviewMesh)
	{
		PreviewMesh->SetRelativeRotation(BaseRotation);
	}
}

void AInspectMeshActor::CaptureNow()
{
	if (Capture)
	{
		Capture->CaptureScene();
	}
}

