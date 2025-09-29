// Copyright Anton Vasserman, All Rights Reserved.


#include "Settings/Collections/CL_AudioSettingsCollection.h"

#include "GameSettingValueScalarDynamic.h"
#include "DataSource/GameSettingDataSourceDynamic.h"
#include "EditCondition/WhenPlayingAsPrimaryPlayer.h"
#include "Player/CL_LocalPlayer.h"
#include "Settings/CL_GameLocalUserSettings.h"

#define LOCTEXT_NAMESPACE "CL"

//~ UCL_SettingsCollection Begin

void UCL_AudioSettingsCollection::InitializeSettings(UCL_LocalPlayer* InLocalPlayer)
{
	SetDevName(TEXT("AudioCollection"));
	SetDisplayName(LOCTEXT("AudioCollection_Name", "Audio"));
	Initialize(InLocalPlayer);

	// Volume
	{
		UGameSettingCollection* VolumeSettings = NewObject<UGameSettingCollection>();
		VolumeSettings->SetDevName(TEXT("VolumeCollection"));
		VolumeSettings->SetDisplayName(LOCTEXT("VolumeCollection_Name", "Volume"));
		AddSetting(VolumeSettings);

		// MasterVolume
		{
			UGameSettingValueScalarDynamic* MasterVolumeSetting = NewObject<UGameSettingValueScalarDynamic>();
			MasterVolumeSetting->SetDevName(TEXT("MasterVolume"));
			MasterVolumeSetting->SetDisplayName(LOCTEXT("MasterVolume_Name", "Master"));
			MasterVolumeSetting->SetDescriptionRichText(LOCTEXT("MasterVolume_Description", "Adjusts the volume of everything."));

			MasterVolumeSetting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetMasterVolume));
			MasterVolumeSetting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetMasterVolume));
			MasterVolumeSetting->SetDefaultValue(GetDefault<UCL_GameLocalUserSettings>()->GetMasterVolume());
			MasterVolumeSetting->SetDisplayFormat(UGameSettingValueScalarDynamic::ZeroToOnePercent);

			MasterVolumeSetting->AddEditCondition(FWhenPlayingAsPrimaryPlayer::Get());

			VolumeSettings->AddSetting(MasterVolumeSetting);
		}

		// MusicVolume
		{
			UGameSettingValueScalarDynamic* MusicVolumeSetting = NewObject<UGameSettingValueScalarDynamic>();
			MusicVolumeSetting->SetDevName(TEXT("MusicVolume"));
			MusicVolumeSetting->SetDisplayName(LOCTEXT("MusicVolume_Name", "Music"));
			MusicVolumeSetting->SetDescriptionRichText(LOCTEXT("MusicVolume_Description", "Adjusts the music volume."));

			MusicVolumeSetting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetMusicVolume));
			MusicVolumeSetting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetMusicVolume));
			MusicVolumeSetting->SetDefaultValue(GetDefault<UCL_GameLocalUserSettings>()->GetMusicVolume());
			MusicVolumeSetting->SetDisplayFormat(UGameSettingValueScalarDynamic::ZeroToOnePercent);

			MusicVolumeSetting->AddEditCondition(FWhenPlayingAsPrimaryPlayer::Get());

			VolumeSettings->AddSetting(MusicVolumeSetting);
		}

		// EffectsVolume
		{
			UGameSettingValueScalarDynamic* EffectsVolumeSetting = NewObject<UGameSettingValueScalarDynamic>();
			EffectsVolumeSetting->SetDevName(TEXT("EffectsVolume"));
			EffectsVolumeSetting->SetDisplayName(LOCTEXT("EffectsVolume_Name", "Effects"));
			EffectsVolumeSetting->SetDescriptionRichText(LOCTEXT("EffectsVolume_Description", "Adjusts the effects volume."));

			EffectsVolumeSetting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetSFXVolume));
			EffectsVolumeSetting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetSFXVolume));
			EffectsVolumeSetting->SetDefaultValue(GetDefault<UCL_GameLocalUserSettings>()->GetSFXVolume());
			EffectsVolumeSetting->SetDisplayFormat(UGameSettingValueScalarDynamic::ZeroToOnePercent);

			EffectsVolumeSetting->AddEditCondition(FWhenPlayingAsPrimaryPlayer::Get());

			VolumeSettings->AddSetting(EffectsVolumeSetting);
		}

		// UIVolume
		{
			UGameSettingValueScalarDynamic* UIVolumeSetting = NewObject<UGameSettingValueScalarDynamic>();
			UIVolumeSetting->SetDevName(TEXT("UIVolume"));
			UIVolumeSetting->SetDisplayName(LOCTEXT("UIVolume_Name", "UI"));
			UIVolumeSetting->SetDescriptionRichText(LOCTEXT("UIVolume_Description", "Adjusts the User Interface volume."));

			UIVolumeSetting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetUIVolume));
			UIVolumeSetting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetUIVolume));
			UIVolumeSetting->SetDefaultValue(GetDefault<UCL_GameLocalUserSettings>()->GetUIVolume());
			UIVolumeSetting->SetDisplayFormat(UGameSettingValueScalarDynamic::ZeroToOnePercent);

			UIVolumeSetting->AddEditCondition(FWhenPlayingAsPrimaryPlayer::Get());

			VolumeSettings->AddSetting(UIVolumeSetting);
		}
	}
}

//~ UCL_SettingsCollection End

#undef LOCTEXT_NAMESPACE
