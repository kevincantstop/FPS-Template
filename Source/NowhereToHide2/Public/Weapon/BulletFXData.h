#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BulletFXData.generated.h"

class UMaterialInterface;
class USoundBase;
class UParticleSystem;

USTRUCT(BlueprintType)
struct FBulletFXEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	TObjectPtr<UMaterialInterface> DecalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	TObjectPtr<UParticleSystem> ImpactVFX;
};

UCLASS(BlueprintType)
class NOWHERETOHIDE2_API UBulletFXData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	TMap<FName, FBulletFXEntry> Entries;
	
	const FBulletFXEntry* FindEntry(FName RowName) const
	{
		return Entries.Find(RowName);
	}
};