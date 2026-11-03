#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class NOWHERETOHIDE2_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

	USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	
	void AttachToMesh(USkeletalMeshComponent* ParentMesh, FName SocketName);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NowhereToHide|Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;
};
