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

			EffectsVolumeSetting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetEffectsVolume));
			EffectsVolumeSetting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetEffectsVolume));
			EffectsVolumeSetting->SetDefaultValue(GetDefault<UCL_GameLocalUserSettings>()->GetEffectsVolume());
			EffectsVolumeSetting->SetDisplayFormat(UGameSettingValueScalarDynamic::ZeroToOnePercent);

			EffectsVolumeSetting->AddEditCondition(FWhenPlayingAsPrimaryPlayer::Get());

			VolumeSettings->AddSetting(EffectsVolumeSetting);
		}

		// VoiceVolume
		{
			UGameSettingValueScalarDynamic* VoiceVolumeSetting = NewObject<UGameSettingValueScalarDynamic>();
			VoiceVolumeSetting->SetDevName(TEXT("VoiceVolume"));
			VoiceVolumeSetting->SetDisplayName(LOCTEXT("VoiceVolume_Name", "Voice"));
			VoiceVolumeSetting->SetDescriptionRichText(LOCTEXT("VoiceVolume_Description", "Adjusts the voice volume."));

			VoiceVolumeSetting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetVoiceVolume));
			VoiceVolumeSetting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetVoiceVolume));
			VoiceVolumeSetting->SetDefaultValue(GetDefault<UCL_GameLocalUserSettings>()->GetVoiceVolume());
			VoiceVolumeSetting->SetDisplayFormat(UGameSettingValueScalarDynamic::ZeroToOnePercent);

			VoiceVolumeSetting->AddEditCondition(FWhenPlayingAsPrimaryPlayer::Get());

			VolumeSettings->AddSetting(VoiceVolumeSetting);
		}
	}
}

//~ UCL_SettingsCollection End
