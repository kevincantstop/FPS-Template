#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class NOWHERETOHIDE2_API IInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "NowhereToHide|Interaction")
	void Interact(APawn* Interactor);
	virtual void Interact_Implementation(APawn* Interactor) {}
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "NowhereToHide|Interaction")
	bool CanInteract(APawn* Interactor) const;
	virtual bool CanInteract_Implementation(APawn* Interactor) const { return true; }
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "NowhereToHide|Interaction")
	FText GetInteractionPrompt() const;
	virtual FText GetInteractionPrompt_Implementation() const { return FText::GetEmpty(); }
};