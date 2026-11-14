#include "Interaction/HealthPickup.h"

#include "Health/HealthComponent.h"
#include "Player/PlayerCharacter.h"

AHealthPickup::AHealthPickup()
{
	bOneShot = true;
}

void AHealthPickup::Interact_Implementation(APawn* Interactor)
{
	Super::Interact_Implementation(Interactor);

	if (APlayerCharacter* Player = Cast<APlayerCharacter>(Interactor))
	{
		if (UHealthComponent* Health = Player->GetHealthComponent())
		{
			Health->Heal(HealAmount);
		}
	}
	
	Destroy();
}