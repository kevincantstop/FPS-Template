#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DiaryData.generated.h"

class UStaticMesh;

USTRUCT(BlueprintType)
struct FDiaryEntry
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Diary")
	TObjectPtr<UStaticMesh> Mesh;
	
	UPROPERTY(EditAnywhere, Category = "Diary")
	FRotator InitialRotation = FRotator(0.f, -30.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "Diary")
	FText Title;

	UPROPERTY(EditAnywhere, Category = "Diary", meta = (MultiLine = "true"))
	FText Body;
};

UCLASS(BlueprintType)
class NOWHERETOHIDE2_API UDiaryData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Diary")
	TMap<FName, FDiaryEntry> Entries;
	
	UFUNCTION(BlueprintPure, Category = "Diary")
	bool FindEntry(FName DiaryID, FDiaryEntry& OutEntry) const;

	UFUNCTION(BlueprintPure, Category = "Diary")
	bool ContainsEntry(FName DiaryID) const { return Entries.Contains(DiaryID); }
};