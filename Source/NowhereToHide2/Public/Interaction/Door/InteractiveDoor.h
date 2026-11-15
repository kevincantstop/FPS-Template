#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractableActor.h"
#include "InteractiveDoor.generated.h"

class USceneComponent;
class USoundBase;

UCLASS()
class NOWHERETOHIDE2_API AInteractiveDoor : public AInteractableActor
{
	GENERATED_BODY()

public:
	AInteractiveDoor();

	//~ Begin IInteractable
	virtual bool  CanInteract_Implementation(APawn* Interactor) const override;
	virtual void  Interact_Implementation(APawn* Interactor) override;
	virtual FText GetInteractionPrompt_Implementation() const override;
	//~ End IInteractable

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NowhereToHide|Door")
	TObjectPtr<USceneComponent> Hinge;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NowhereToHide|Door")
	TObjectPtr<UStaticMeshComponent> DoorPanel;
	
	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Door")
	float OpenAngle = 90.f;
	
	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Door", meta = (ClampMin = "0.05"))
	float OpenDuration = 1.2f;
	
	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Door")
	bool bCanClose = true;

	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Door")
	TObjectPtr<USoundBase> DoorSound;

	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Door")
	FText OpenPrompt;

	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Door")
	FText ClosePrompt;

private:
	void StartSwing(bool bNewOpen);
	void FinishSwing();

	FRotator ClosedHingeRotation = FRotator::ZeroRotator;
	float StartYaw = 0.f;
	float TargetYaw = 0.f;
	float AnimElapsed = 0.f;

	bool bOpen = false;
	bool bAnimating = false;
};