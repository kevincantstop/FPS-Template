#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/Interactable.h"
#include "InteractableActor.generated.h"

class UStaticMeshComponent;

UCLASS(Abstract, Blueprintable)
class NOWHERETOHIDE2_API AInteractableActor : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	AInteractableActor();
	
	virtual bool  CanInteract_Implementation(APawn* Interactor) const override;
	virtual void  Interact_Implementation(APawn* Interactor) override;
	virtual FText GetInteractionPrompt_Implementation() const override;

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Interaction")
	bool IsInteractable() const { return bInteractable; }

	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Interaction")
	void SetInteractable(bool bNewInteractable);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "NowhereToHide|Interaction")
	void OnInteracted(APawn* Interactor);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NowhereToHide|Interaction")
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NowhereToHide|Interaction")
	FText PromptText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NowhereToHide|Interaction")
	bool bInteractable = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NowhereToHide|Interaction")
	bool bOneShot = false;
};