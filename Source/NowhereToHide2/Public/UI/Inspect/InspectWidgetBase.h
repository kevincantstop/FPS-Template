#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InspectWidgetBase.generated.h"

class AInspectItem;

UCLASS()
class NOWHERETOHIDE2_API UInspectWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Inspect")
	void SetInspectItem(AInspectItem* InItem);

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Inspect")
	AInspectItem* GetInspectItem() const { return InspectItem; }

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "NowhereToHide|Inspect")
	void OnInspectItemSet(AInspectItem* Item);

	UPROPERTY(BlueprintReadOnly, Category = "NowhereToHide|Inspect")
	TObjectPtr<AInspectItem> InspectItem;
};