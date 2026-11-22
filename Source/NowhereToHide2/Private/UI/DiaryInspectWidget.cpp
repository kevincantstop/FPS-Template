#include "UI/DiaryInspectWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Game/GamePlaySubsystem.h"
#include "Interaction/InspectMeshActor.h"
#include "UI/DiaryData.h"

void UDiaryInspectWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (UWorld* World = GetWorld())
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		PreviewActor = World->SpawnActor<AInspectMeshActor>(
			AInspectMeshActor::StaticClass(),
			FVector(0.f, 0.f, 100000.f),
			FRotator::ZeroRotator,
			Params);
	}

	if (PreviewImage && PreviewRenderTarget)
	{
		PreviewImage->SetBrushResourceObject(PreviewRenderTarget);
	}
}

void UDiaryInspectWidget::NativeDestruct()
{
	DestroyPreviewActor();
	Super::NativeDestruct();
}

void UDiaryInspectWidget::DestroyPreviewActor()
{
	if (PreviewActor)
	{
		PreviewActor->Destroy();
		PreviewActor = nullptr;
	}
}

void UDiaryInspectWidget::ShowDiary(FName DiaryID)
{
	if (!DiaryDatabase)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Diary] DiaryDatabase is not set on the widget"));
		return;
	}

	const FDiaryEntry* Entry = DiaryDatabase->Entries.Find(DiaryID);
	if (!Entry)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Diary] Entry not found: %s"), *DiaryID.ToString());
		return;
	}

	if (TitleText) { TitleText->SetText(Entry->Title); }
	if (BodyText)  { BodyText->SetText(Entry->Body); }

	if (PreviewActor)
	{
		PreviewActor->SetPreviewMesh(Entry->Mesh, Entry->InitialRotation);
		PreviewActor->CaptureNow();
	}
}

void UDiaryInspectWidget::SetAutoRotate(bool bEnabled)
{
	bAutoRotate = bEnabled;
}

void UDiaryInspectWidget::CloseDiary()
{
	if (UGamePlaySubsystem* Flow = GetGameInstance() ? GetGameInstance()->GetSubsystem<UGamePlaySubsystem>() : nullptr)
	{
		Flow->SetGamePaused(false);
	}

	RemoveFromParent();
}

void UDiaryInspectWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (PreviewActor && bAutoRotate && !bIsDragging)
	{
		PreviewActor->AddPreviewRotation(AutoRotateSpeed * InDeltaTime, 0.f);
		CaptureIfNeeded(InDeltaTime);
	}
}

void UDiaryInspectWidget::CaptureIfNeeded(float DeltaTime)
{
	CaptureAccumulator += DeltaTime;

	const float Interval = 1.f / FMath::Max(CaptureRate, 1.f);
	if (CaptureAccumulator >= Interval)
	{
		CaptureAccumulator = 0.f;
		if (PreviewActor) { PreviewActor->CaptureNow(); }
	}
}

FReply UDiaryInspectWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() != EKeys::LeftMouseButton) { return FReply::Unhandled(); }
	
	if (PreviewImage && PreviewImage->GetCachedGeometry().IsUnderLocation(InMouseEvent.GetScreenSpacePosition()))
	{
		bIsDragging = true;
		return FReply::Handled().CaptureMouse(TakeWidget());
	}

	return FReply::Unhandled();
}

FReply UDiaryInspectWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!bIsDragging) { return FReply::Unhandled(); }

	bIsDragging = false;
	return FReply::Handled().ReleaseMouseCapture();
}

FReply UDiaryInspectWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!bIsDragging || !PreviewActor) { return FReply::Unhandled(); }

	const FVector2D Delta = InMouseEvent.GetCursorDelta();

	PreviewActor->AddPreviewRotation(Delta.X * DragSensitivity, -Delta.Y * DragSensitivity);
	PreviewActor->CaptureNow();

	return FReply::Handled();
}