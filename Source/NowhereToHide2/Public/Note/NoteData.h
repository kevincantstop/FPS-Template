#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NoteData.generated.h"

class UStaticMesh;
class UTexture2D;

USTRUCT(BlueprintType)
struct FNoteEntry
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Note")
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Note")
	FRotator InitialRotation = FRotator(0.f, -30.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Note")
	FText Title;

	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Note", meta = (MultiLine = "true"))
	FText Body;
	
	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Note")
	TObjectPtr<UTexture2D> Image;
	
	UPROPERTY(EditAnywhere, Category = "NowhereToHide|Note")
	FText Subtitle;
};

UCLASS(BlueprintType)
class NOWHERETOHIDE2_API UNoteData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NowhereToHide|Note")
	TMap<FName, FNoteEntry> Entries;

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Note")
	bool FindEntry(FName InNoteID, FNoteEntry& OutEntry) const;

	UFUNCTION(BlueprintPure, Category = "NowhereToHide|Note")
	bool ContainsEntry(FName InNoteID) const { return Entries.Contains(InNoteID); }
};