// Copyright Anton Vasserman, All Rights Reserved.


#include "Settings/CL_GameLocalUserSettings.h"

#include "AudioModulationStatics.h"
#include "SoundControlBusMix.h"
#include "Audio/CL_AudioSettings.h"
#include "Characters/CL_PlayerCharacterCameraMode.h"

UCL_GameLocalUserSettings::UCL_GameLocalUserSettings()
{
	SetToDefaults();
}

UCL_GameLocalUserSettings* UCL_GameLocalUserSettings::Get()
{
	return GEngine ? CastChecked<UCL_GameLocalUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

// Begin Video Settings
float UCL_GameLocalUserSettings::GetDisplayGamma() const
{
	return DisplayGamma;
}

void UCL_GameLocalUserSettings::SetDisplayGamma(float InGamma)
{
	DisplayGamma = InGamma;
	ApplyDisplayGamma();
}

void UCL_GameLocalUserSettings::ApplyDisplayGamma()
{
	if (GEngine)
	{
		GEngine->DisplayGamma = DisplayGamma;
	}
}

// End Video Settings

// Begin Game Settings
ECL_PlayerCharacterCameraMode UCL_GameLocalUserSettings::GetCameraDistance() const
{
	return CameraDistance;
}

void UCL_GameLocalUserSettings::SetCameraDistance(ECL_PlayerCharacterCameraMode InCameraDistance)
{
	CameraDistance = InCameraDistance;
}
// End Game Settings

// Begin Audio Settings
float UCL_GameLocalUserSettings::GetMasterVolume() const
{
	return MasterVolume;
}

void UCL_GameLocalUserSettings::SetMasterVolume(float InVolume)
{
	MasterVolume = InVolume;

	if (!bSoundControlBusMixLoaded)
	{
		LoadUserControlBusMix();
	}

	ensureMsgf(bSoundControlBusMixLoaded, TEXT("UserControlBusMix Settings Failed to Load."));

	if (const TObjectPtr<USoundControlBus>* ControlBusDblPtr = ControlBusMap.Find(TEXT("Master")))
	{
		if (USoundControlBus* ControlBusPtr = *ControlBusDblPtr)
		{
			SetVolumeForControlBus(ControlBusPtr, MasterVolume);
		}
	}
}

float UCL_GameLocalUserSettings::GetMusicVolume() const
{
	return MusicVolume;
}

void UCL_GameLocalUserSettings::SetMusicVolume(float InVolume)
{
	MusicVolume = InVolume;

	ensureMsgf(bSoundControlBusMixLoaded, TEXT("UserControlBusMix Settings Failed to Load."));

	if (const TObjectPtr<USoundControlBus>* ControlBusDblPtr = ControlBusMap.Find(TEXT("Music")))
	{
		if (USoundControlBus* ControlBusPtr = *ControlBusDblPtr)
		{
			SetVolumeForControlBus(ControlBusPtr, MusicVolume);
		}
	}
}

float UCL_GameLocalUserSettings::GetSFXVolume() const
{
	return SFXVolume;
}

void UCL_GameLocalUserSettings::SetSFXVolume(float InVolume)
{
	SFXVolume = InVolume;

	ensureMsgf(bSoundControlBusMixLoaded, TEXT("UserControlBusMix Settings Failed to Load."));

	if (const TObjectPtr<USoundControlBus>* ControlBusDblPtr = ControlBusMap.Find(TEXT("SFX")))
	{
		if (USoundControlBus* ControlBusPtr = *ControlBusDblPtr)
		{
			SetVolumeForControlBus(ControlBusPtr, SFXVolume);
		}
	}
}

float UCL_GameLocalUserSettings::GetUIVolume() const
{
	return UIVolume;
}

void UCL_GameLocalUserSettings::SetUIVolume(float InVolume)
{
	UIVolume = InVolume;
	
	ensureMsgf(bSoundControlBusMixLoaded, TEXT("UserControlBusMix Settings Failed to Load."));

	if (const TObjectPtr<USoundControlBus>* ControlBusDblPtr = ControlBusMap.Find(TEXT("UI")))
	{
		if (USoundControlBus* ControlBusPtr = *ControlBusDblPtr)
		{
			SetVolumeForControlBus(ControlBusPtr, UIVolume);
		}
	}
}

void UCL_GameLocalUserSettings::LoadUserControlBusMix()
{
	if (!GEngine)
	{
		return;
	}

	if (const UWorld* World = GEngine->GetCurrentPlayWorld())
	{
		if (const UCL_AudioSettings* AudioSettings = GetDefault<UCL_AudioSettings>())
		{
			ControlBusMap.Empty();
			
			USoundControlBus* MasterControlBus = nullptr;
			if (UObject* ObjPath = AudioSettings->MasterVolumeControlBus.TryLoad())
			{
				if (USoundControlBus* SoundControlBus = Cast<USoundControlBus>(ObjPath))
				{
					MasterControlBus = SoundControlBus;
					ControlBusMap.Add(TEXT("Master"), MasterControlBus);
				}
				else
				{
					ensureMsgf(SoundControlBus, TEXT("Master Control Bus reference missing from Audio Settings."));
				}
			}

			USoundControlBus* MusicControlBus = nullptr;
			if (UObject* ObjPath = AudioSettings->MusicVolumeControlBus.TryLoad())
			{
				if (USoundControlBus* SoundControlBus = Cast<USoundControlBus>(ObjPath))
				{
					MusicControlBus = SoundControlBus;
					ControlBusMap.Add(TEXT("Music"), MusicControlBus);
				}
				else
				{
					ensureMsgf(SoundControlBus, TEXT("Music Control Bus reference missing from Audio Settings."));
				}
			}
			
			USoundControlBus* SFXControlBus = nullptr;
			if (UObject* ObjPath = AudioSettings->SFXVolumeControlBus.TryLoad())
			{
				if (USoundControlBus* SoundControlBus = Cast<USoundControlBus>(ObjPath))
				{
					SFXControlBus = SoundControlBus;
					ControlBusMap.Add(TEXT("SFX"), SFXControlBus);
				}
				else
				{
					ensureMsgf(SoundControlBus, TEXT("SFX Control Bus reference missing from Audio Settings."));
				}
			}
			
			USoundControlBus* UIControlBus = nullptr;
			if (UObject* ObjPath = AudioSettings->UIVolumeControlBus.TryLoad())
			{
				if (USoundControlBus* SoundControlBus = Cast<USoundControlBus>(ObjPath))
				{
					UIControlBus = SoundControlBus;
					ControlBusMap.Add(TEXT("UI"), UIControlBus);
				}
				else
				{
					ensureMsgf(SoundControlBus, TEXT("UI Control Bus reference missing from Audio Settings."));
				}
			}
			
			if (UObject* ObjPath = AudioSettings->UserSettingsControlBusMix.TryLoad())
			{
				if (USoundControlBusMix* SoundControlBusMix = Cast<USoundControlBusMix>(ObjPath))
				{
					ControlBusMix = SoundControlBusMix;

					const FSoundControlBusMixStage MasterControlBusMixStage = UAudioModulationStatics::CreateBusMixStage(World, MasterControlBus, MasterVolume);
					const FSoundControlBusMixStage MusicControlBusMixStage = UAudioModulationStatics::CreateBusMixStage(World, MusicControlBus, MusicVolume);
					const FSoundControlBusMixStage SFXControlBusMixStage = UAudioModulationStatics::CreateBusMixStage(World, SFXControlBus, SFXVolume);
					const FSoundControlBusMixStage UIControlBusMixStage = UAudioModulationStatics::CreateBusMixStage(World, UIControlBus, UIVolume);

					TArray<FSoundControlBusMixStage> ControlBusMixStageArray;
					ControlBusMixStageArray.Add(MasterControlBusMixStage);
					ControlBusMixStageArray.Add(MusicControlBusMixStage);
					ControlBusMixStageArray.Add(SFXControlBusMixStage);
					ControlBusMixStageArray.Add(UIControlBusMixStage);

					UAudioModulationStatics::UpdateMix(World, ControlBusMix, ControlBusMixStageArray);
					
					bSoundControlBusMixLoaded = true;
				}
				else
				{
					ensureMsgf(SoundControlBusMix, TEXT("User Settings Control Bus Mix reference missing from Audio Settings."));
				}
			}
		}
	}
}

void UCL_GameLocalUserSettings::SetVolumeForControlBus(USoundControlBus* InSoundControlBus, float InVolume)
{
	if (!bSoundControlBusMixLoaded)
	{
		LoadUserControlBusMix();
	}

	ensureMsgf(bSoundControlBusMixLoaded, TEXT("UserControlBusMix Settings Failed to Load."));

	if (GEngine && InSoundControlBus && bSoundControlBusMixLoaded)
	{
		if (const UWorld* AudioWorld = GEngine->GetCurrentPlayWorld())
		{
			ensureMsgf(ControlBusMix, TEXT("Control Bus Mix failed to load."));

			// Create and set the Control Bus Mix Stage Parameters
			FSoundControlBusMixStage UpdatedControlBusMixStage;
			UpdatedControlBusMixStage.Bus = InSoundControlBus;
			UpdatedControlBusMixStage.Value.TargetValue = InVolume;
			UpdatedControlBusMixStage.Value.AttackTime = 0.01f;
			UpdatedControlBusMixStage.Value.ReleaseTime = 0.01f;

			// Add the Control Bus Mix Stage to an Array as the UpdateMix function requires
			TArray<FSoundControlBusMixStage> UpdatedMixStageArray;
			UpdatedMixStageArray.Add(UpdatedControlBusMixStage);

			// Modify the matching bus Mix Stage parameters on the User Control Bus Mix
			UAudioModulationStatics::UpdateMix(AudioWorld, ControlBusMix, UpdatedMixStageArray);
		}
	}
}
// End Audio Settings

// Begin UI Settings
bool UCL_GameLocalUserSettings::GetRotateMinimap() const
{
	return bRotateMinimap;
}

void UCL_GameLocalUserSettings::SetRotateMinimap(bool bInRotateMinimap)
{
	bRotateMinimap = bInRotateMinimap;
	OnRotateMinimapChanged.Broadcast(bRotateMinimap);
}
// End UI Settings

// Begin Accessibility Settings
EColorBlindMode UCL_GameLocalUserSettings::GetColorBlindMode() const
{
	return ColorBlindMode;
}

void UCL_GameLocalUserSettings::SetColorBlindMode(EColorBlindMode InMode)
{
	if (ColorBlindMode != InMode)
	{
		ColorBlindMode = InMode;
		FSlateApplication::Get().GetRenderer()->SetColorVisionDeficiencyType(
			static_cast<EColorVisionDeficiency>(static_cast<int32>(ColorBlindMode)), ColorBlindStrength, true, false);
	}
}

int32 UCL_GameLocalUserSettings::GetColorBlindStrength() const
{
	return ColorBlindStrength;
}

void UCL_GameLocalUserSettings::SetColorBlindStrength(int32 InColorBlindStrength)
{
	InColorBlindStrength = FMath::Clamp(InColorBlindStrength, 0, 10);
	if (ColorBlindStrength != InColorBlindStrength)
	{
		ColorBlindStrength = InColorBlindStrength;
		FSlateApplication::Get().GetRenderer()->SetColorVisionDeficiencyType(
			static_cast<EColorVisionDeficiency>(static_cast<int32>(ColorBlindMode)), ColorBlindStrength, true, false);
	}
}
// End Accessibility Settings

//~ UGameUserSettings Begin
void UCL_GameLocalUserSettings::SetToDefaults()
{
	Super::SetToDefaults();

	bSoundControlBusMixLoaded = false;
}

void UCL_GameLocalUserSettings::ApplyNonResolutionSettings()
{
	Super::ApplyNonResolutionSettings();

	if (!bSoundControlBusMixLoaded)
	{
		LoadUserControlBusMix();
	}

	if (TObjectPtr<USoundControlBus>* ControlBusDblPtr = ControlBusMap.Find(TEXT("Master")))
	{
		if (USoundControlBus* ControlBusPtr = *ControlBusDblPtr)
		{
			SetVolumeForControlBus(ControlBusPtr, MasterVolume);
		}
	}

	if (TObjectPtr<USoundControlBus>* ControlBusDblPtr = ControlBusMap.Find(TEXT("Music")))
	{
		if (USoundControlBus* ControlBusPtr = *ControlBusDblPtr)
		{
			SetVolumeForControlBus(ControlBusPtr, MusicVolume);
		}
	}

	if (TObjectPtr<USoundControlBus>* ControlBusDblPtr = ControlBusMap.Find(TEXT("SFX")))
	{
		if (USoundControlBus* ControlBusPtr = *ControlBusDblPtr)
		{
			SetVolumeForControlBus(ControlBusPtr, SFXVolume);
		}
	}

	if (TObjectPtr<USoundControlBus>* ControlBusDblPtr = ControlBusMap.Find(TEXT("UI")))
	{
		if (USoundControlBus* ControlBusPtr = *ControlBusDblPtr)
		{
			SetVolumeForControlBus(ControlBusPtr, UIVolume);
		}
	}

	if (FApp::CanEverRender())
	{
		ApplyDisplayGamma();
	}
}
//~ UGameUserSettings End
