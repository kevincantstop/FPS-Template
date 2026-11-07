// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NowhereToHideGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class NOWHERETOHIDE2_API ANowhereToHideGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "NowhereToHide|Audio", meta = (ClampMin = "0.0", ClampMax = "2.0"))
	float MasterVolume = 0.6f;
	
	UFUNCTION(BlueprintCallable, Category = "NowhereToHide|Audio")
	void SetMasterVolume(float NewVolume);
	
	UFUNCTION(Exec)
	void SetGameVolume(float NewVolume);

protected:
	virtual void BeginPlay() override;   // 如果还没有的话加上
};
