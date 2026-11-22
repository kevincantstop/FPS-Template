#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractableActor.h"
#include "InteractiveDiary.generated.h"

class UDiaryInspectWidget;

UCLASS()
class NOWHERETOHIDE2_API AInteractiveDiary : public AInteractableActor
{
	GENERATED_BODY()

public:
	AInteractiveDiary();
	
	virtual void Interact_Implementation(APawn* Interactor) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NowhereToHide|Diary")
	FName DiaryID = TEXT("Diary_01");
	
	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Diary")
	TSubclassOf<UDiaryInspectWidget> DiaryWidgetClass;
	
	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Diary")
	bool bPauseGameWhenOpened = true;

private:
	UPROPERTY(Transient)
	TObjectPtr<UDiaryInspectWidget> OpenedWidget;
};