#include "Game/NowhereToHideGameModeBase.h"
#include "AudioDevice.h"

void ANowhereToHideGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SetMasterVolume(MasterVolume);
}

void ANowhereToHideGameModeBase::SetMasterVolume(float NewVolume)
{
	if (UWorld* World = GetWorld())
	{
		if (FAudioDevice* AudioDevice = World->GetAudioDeviceRaw())
		{
			AudioDevice->SetTransientPrimaryVolume(FMath::Clamp(NewVolume, 0.f, 2.f));
		}
	}
}

void ANowhereToHideGameModeBase::SetGameVolume(float NewVolume)
{
	SetMasterVolume(NewVolume);
}
