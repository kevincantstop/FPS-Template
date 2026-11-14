#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractableActor.h"
#include "MedicalPack.generated.h"

/**
 * 医疗包：拾取后立刻回血并消失。
 * 以后接背包系统时，把 Heal 换成 AddItem 即可。
 */
UCLASS()
class NOWHERETOHIDE2_API AMedicalPack : public AInteractableActor
{
	GENERATED_BODY()

public:
	AMedicalPack();

	//~ Begin IInteractable
	virtual void Interact_Implementation(APawn* Interactor) override;
	//~ End IInteractable

protected:
	/** 回复量 */
	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Pickup", meta = (ClampMin = "0.0"))
	float HealAmount = 25.f;

	/** 拾取后是否销毁 */
	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Pickup")
	bool bDestroyOnPickup = true;
};