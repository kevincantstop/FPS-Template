#include "Interaction/Pickups/AmmoPack.h"

#include "Combat/CombatComponent.h"
#include "Player/PlayerCharacter.h"
#include "Weapon/Weapon.h"

AAmmoPack::AAmmoPack()
{
	bOneShot = true;

	PromptText = FText::FromString(TEXT("[E] Pick up ammo"));
}

void AAmmoPack::Interact_Implementation(APawn* Interactor)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Interactor);
	if (!Player || !Player->GetCombatComponent()) { return; }

	AWeapon* Weapon = Player->GetCombatComponent()->GetCurrentWeapon();
	if (!Weapon) { return; }
	
	Super::Interact_Implementation(Interactor);

	Weapon->AddReserveAmmo(Rounds);

	if (bDestroyOnPickup)
	{
		Destroy();
	}
}