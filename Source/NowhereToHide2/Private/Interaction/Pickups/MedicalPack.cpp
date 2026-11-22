#include "Interaction/Pickups/MedicalPack.h"

#include "Health/HealthComponent.h"
#include "Player/PlayerCharacter.h"

AMedicalPack::AMedicalPack()
{
	bOneShot = true;

	PromptText = FText::FromString(TEXT("Pick up medkit"));
}

void AMedicalPack::Interact_Implementation(APawn* Interactor)
{
	Super::Interact_Implementation(Interactor);

	if (APlayerCharacter* Player = Cast<APlayerCharacter>(Interactor))
	{
		if (UHealthComponent* Health = Player->GetHealthComponent())
		{
			Health->Heal(HealAmount);
		}
	}

	if (bDestroyOnPickup)
	{
		Destroy();
	}
}