#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractableActor.h"
#include "AmmoPack.generated.h"

/**
 * 弹药包：拾取后补充当前武器的备弹并消失。
 */
UCLASS()
class NOWHERETOHIDE2_API AAmmoPack : public AInteractableActor
{
	GENERATED_BODY()

public:
	AAmmoPack();
	
	virtual void Interact_Implementation(APawn* Interactor) override;

protected:
	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Pickup", meta = (ClampMin = "0"))
	int32 Rounds = 30;
	
	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Pickup")
	bool bDestroyOnPickup = true;
};