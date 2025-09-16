// Copyright Anton Vasserman, All Rights Reserved.


#include "Audio/CL_AudioMixSubsystem.h"

#include "AudioModulationStatics.h"
#include "SoundControlBus.h"
#include "SoundControlBusMix.h"
#include "Audio/CL_AudioSettings.h"
#include "Settings/CL_GameLocalUserSettings.h"

//~ UWorldSubsystem Begin

void UCL_AudioMixSubsystem::PostInitialize()
{
	if (const UCL_AudioSettings* AudioSettings = GetDefault<UCL_AudioSettings>())
	{
		if (UObject* ObjPath = AudioSettings->UserSettingsControlBusMix.TryLoad())
		{
			if (USoundControlBusMix* SoundControlBusMix = Cast<USoundControlBusMix>(ObjPath))
			{
				UserMix = SoundControlBusMix;
			}
			else
			{
				ensureMsgf(SoundControlBusMix, TEXT("User Control Bus Mix reference missing from Audio Settings."));
			}
		}

		if (UObject* ObjPath = AudioSettings->MasterVolumeControlBus.TryLoad())
		{
			if (USoundControlBus* SoundControlBus = Cast<USoundControlBus>(ObjPath))
			{
				MasterControlBus = SoundControlBus;
			}
			else
			{
				ensureMsgf(SoundControlBus, TEXT("Master Control Bus reference missing from Audio Settings."));
			}
		}

		if (UObject* ObjPath = AudioSettings->MusicVolumeControlBus.TryLoad())
		{
			if (USoundControlBus* SoundControlBus = Cast<USoundControlBus>(ObjPath))
			{
				MusicControlBus = SoundControlBus;
			}
			else
			{
				ensureMsgf(SoundControlBus, TEXT("Music Control Bus reference missing from Audio Settings."));
			}
		}

		if (UObject* ObjPath = AudioSettings->SFXVolumeControlBus.TryLoad())
		{
			if (USoundControlBus* SoundControlBus = Cast<USoundControlBus>(ObjPath))
			{
				SFXControlBus = SoundControlBus;
			}
			else
			{
				ensureMsgf(SoundControlBus, TEXT("SoundFX Control Bus reference missing from Audio Settings."));
			}
		}

		if (UObject* ObjPath = AudioSettings->VoiceVolumeControlBus.TryLoad())
		{
			if (USoundControlBus* SoundControlBus = Cast<USoundControlBus>(ObjPath))
			{
				VoiceControlBus = SoundControlBus;
			}
			else
			{
				ensureMsgf(SoundControlBus, TEXT("Dialogue Control Bus reference missing from Audio Settings."));
			}
		}
	}
}

void UCL_AudioMixSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	if (const UWorld* World = InWorld.GetWorld())
	{
		if (const UCL_GameLocalUserSettings* LocalUserSettings = GetDefault<UCL_GameLocalUserSettings>())
		{
			if (UserMix)
			{
				UAudioModulationStatics::ActivateBusMix(World, UserMix);

				if (MasterControlBus && MusicControlBus && SFXControlBus && VoiceControlBus)
				{
					const FSoundControlBusMixStage MasterControlBusMixStage = UAudioModulationStatics::CreateBusMixStage(World, MasterControlBus, LocalUserSettings->GetMasterVolume());
					const FSoundControlBusMixStage MusicControlBusMixStage = UAudioModulationStatics::CreateBusMixStage(World, MusicControlBus, LocalUserSettings->GetMusicVolume());
					const FSoundControlBusMixStage SFXControlBusMixStage = UAudioModulationStatics::CreateBusMixStage(World, SFXControlBus, LocalUserSettings->GetSFXVolume());
					const FSoundControlBusMixStage VoiceControlBusMixStage = UAudioModulationStatics::CreateBusMixStage(World, VoiceControlBus, LocalUserSettings->GetVoiceVolume());

					TArray<FSoundControlBusMixStage> ControlBusMixStageArray;
					ControlBusMixStageArray.Add(MasterControlBusMixStage);
					ControlBusMixStageArray.Add(MusicControlBusMixStage);
					ControlBusMixStageArray.Add(SFXControlBusMixStage);
					ControlBusMixStageArray.Add(VoiceControlBusMixStage);

					UAudioModulationStatics::UpdateMix(World, UserMix, ControlBusMixStageArray);
				}
			}
		}
	}
}

//~ UWorldSubsystem End
