#pragma once

#include "CoreMinimal.h"
#include "Interaction/Inspect/InspectItem.h"
#include "NoteItem.generated.h"

class UNoteData;
class UStaticMesh;

UCLASS()
class NOWHERETOHIDE2_API ANoteItem : public AInspectItem
{
	GENERATED_BODY()

public:
	ANoteItem();

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Note")
	FName GetNoteID() const { return NoteID; }

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Note")
	FText GetNoteTitle() const { return CachedTitle; }

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Note")
	FText GetNoteSubtitle() const { return CachedSubtitle; }

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Note")
	FText GetNoteBody() const { return CachedBody; }

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Note")
	FText GetNoteBodyRaw() const { return CachedBody; }
	
	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Note")
	void RefreshFromDatabase();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NowhereToHide|Note")
	FName NoteID = TEXT("note_01");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NowhereToHide|Note")
	TObjectPtr<UNoteData> NoteDatabase;

private:
	UPROPERTY(BlueprintReadOnly, Category = "NowhereToHide|Note", meta = (AllowPrivateAccess = "true"))
	FText CachedTitle;

	UPROPERTY(BlueprintReadOnly, Category = "NowhereToHide|Note", meta = (AllowPrivateAccess = "true"))
	FText CachedSubtitle;

	UPROPERTY(BlueprintReadOnly, Category = "NowhereToHide|Note", meta = (AllowPrivateAccess = "true"))
	FText CachedBody;
};