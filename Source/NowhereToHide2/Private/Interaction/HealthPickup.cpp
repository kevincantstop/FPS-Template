#include "Interaction/HealthPickup.h"

#include "Health/HealthComponent.h"
#include "Player/PlayerCharacter.h"

AHealthPickup::AHealthPickup()
{
	bOneShot = true;
}

bool AHealthPickup::CanInteract_Implementation(APawn* Interactor) const
{
	if (!Super::CanInteract_Implementation(Interactor)) { return false; }
	
	const APlayerCharacter* Player = Cast<APlayerCharacter>(Interactor);
	if (!Player || !Player->GetHealthComponent()) { return false; }

	return Player->GetHealthComponent()->GetHealth() < Player->GetHealthComponent()->GetMaxHealth();
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

	if (bDestroyOnPickup)
	{
		Destroy();
	}
}