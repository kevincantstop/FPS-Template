#pragma once

#include "InteractableActor.h"
#include "HealthPickup.generated.h"

UCLASS()
class NOWHERETOHIDE2_API AHealthPickup : public AInteractableActor
{
	GENERATED_BODY()

public:
	AHealthPickup();

	virtual bool CanInteract_Implementation(APawn* Interactor) const override;
	virtual void Interact_Implementation(APawn* Interactor) override;

protected:
	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Pickup", meta = (ClampMin = "0.0"))
	float HealAmount = 25.f;
	
	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Pickup")
	bool bDestroyOnPickup = true;
};