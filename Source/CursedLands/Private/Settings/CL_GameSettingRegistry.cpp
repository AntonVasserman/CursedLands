// Copyright Anton Vasserman, All Rights Reserved.


#include "Settings/CL_GameSettingRegistry.h"

#include "GameSettingCollection.h"
#include "GameSettingValueDiscreteDynamic.h"
#include "GameSettingValueScalarDynamic.h"
#include "Characters/CL_PlayerCharacterCameraMode.h"
#include "DataSource/GameSettingDataSourceDynamic.h"
#include "EditCondition/WhenPlayingAsPrimaryPlayer.h"
#include "GameFramework/GameUserSettings.h"
#include "Player/CL_LocalPlayer.h"
#include "Settings/CL_GameLocalUserSettings.h"

#define LOCTEXT_NAMESPACE "CL"

UCL_GameSettingRegistry* UCL_GameSettingRegistry::Get(UCL_LocalPlayer* InLocalPlayer)
{
	UCL_GameSettingRegistry* Registry = FindObject<UCL_GameSettingRegistry>(InLocalPlayer, TEXT("GameSettingRegistry"), true);
	if (Registry == nullptr)
	{
		Registry = NewObject<UCL_GameSettingRegistry>(InLocalPlayer, TEXT("GameSettingRegistry"));
		Registry->Initialize(InLocalPlayer);
	}

	return Registry;
}

UGameSettingCollection* UCL_GameSettingRegistry::InitializeVideoSettings(UCL_LocalPlayer* InLocalPlayer)
{
	UGameSettingCollection* VideoSettingCollection = NewObject<UGameSettingCollection>();
	VideoSettingCollection->SetDevName(TEXT("VideoCollection"));
	VideoSettingCollection->SetDisplayName(LOCTEXT("VideoCollection_Name", "Video"));
	VideoSettingCollection->Initialize(InLocalPlayer);

	return VideoSettingCollection;
}

UGameSettingCollection* UCL_GameSettingRegistry::InitializeGameSettings(UCL_LocalPlayer* InLocalPlayer)
{
	UGameSettingCollection* GameSettingCollection = NewObject<UGameSettingCollection>();
	GameSettingCollection->SetDevName(TEXT("GameCollection"));
	GameSettingCollection->SetDisplayName(LOCTEXT("GameCollection_Name", "Game"));
	GameSettingCollection->Initialize(InLocalPlayer);

	// Camera
	{
		UGameSettingCollection* CameraSettings = NewObject<UGameSettingCollection>();
		CameraSettings->SetDevName(TEXT("CameraCollection"));
		CameraSettings->SetDisplayName(LOCTEXT("CameraCollection_Name", "Camera"));
		GameSettingCollection->AddSetting(CameraSettings);

		// Camera Distance
		{
			UGameSettingValueDiscreteDynamic_Enum* CameraDistanceSetting = NewObject<UGameSettingValueDiscreteDynamic_Enum>();
			CameraDistanceSetting->SetDevName(TEXT("CameraDistance"));
			CameraDistanceSetting->SetDisplayName(LOCTEXT("CameraDistance_Name", "Camera Distance"));
			CameraDistanceSetting->SetDescriptionRichText(LOCTEXT("CameraDistance_Description", "Switch between default and close over-the-shoulder camera mode."));

			CameraDistanceSetting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetCameraDistance));
			CameraDistanceSetting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetCameraDistance));
			CameraDistanceSetting->AddEnumOption(ECL_PlayerCharacterCameraMode::Default, LOCTEXT("CameraDistanceDefault", "Default"));
			CameraDistanceSetting->AddEnumOption(ECL_PlayerCharacterCameraMode::Close, LOCTEXT("CameraDistanceClose", "Close"));
			CameraDistanceSetting->SetDefaultValue(ECL_PlayerCharacterCameraMode::Default);

			CameraSettings->AddSetting(CameraDistanceSetting);
		}
	}

	// Lorem Ipsum
	{
		UGameSettingCollection* LoremIpsumSettings = NewObject<UGameSettingCollection>();
		LoremIpsumSettings->SetDevName(TEXT("LoremIpsumCollection"));
		LoremIpsumSettings->SetDisplayName(LOCTEXT("LoremIpsumCollection_Name", "Lorem Ipsum"));
		GameSettingCollection->AddSetting(LoremIpsumSettings);

		// Camera Distance 2
		{
			UGameSettingValueDiscreteDynamic_Enum* CameraDistanceSetting = NewObject<UGameSettingValueDiscreteDynamic_Enum>();
			CameraDistanceSetting->SetDevName(TEXT("LoremIpsum"));
			CameraDistanceSetting->SetDisplayName(LOCTEXT("LoremIpsum_Name", "Lorem Ipsum"));
			CameraDistanceSetting->SetDescriptionRichText(LOCTEXT("LoremIpsum_Description", "TODO"));

			CameraDistanceSetting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetCameraDistance));
			CameraDistanceSetting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetCameraDistance));
			CameraDistanceSetting->AddEnumOption(ECL_PlayerCharacterCameraMode::Default, LOCTEXT("CameraDistanceDefault", "Default"));
			CameraDistanceSetting->AddEnumOption(ECL_PlayerCharacterCameraMode::Close, LOCTEXT("CameraDistanceClose", "Close"));
			CameraDistanceSetting->SetDefaultValue(ECL_PlayerCharacterCameraMode::Default);

			LoremIpsumSettings->AddSetting(CameraDistanceSetting);
		}
	}
	
	return GameSettingCollection;
}

UGameSettingCollection* UCL_GameSettingRegistry::InitializeAudioSettings(UCL_LocalPlayer* InLocalPlayer)
{
	UGameSettingCollection* AudioSettingCollection = NewObject<UGameSettingCollection>();
	AudioSettingCollection->SetDevName(TEXT("AudioCollection"));
	AudioSettingCollection->SetDisplayName(LOCTEXT("AudioCollection_Name", "Audio"));
	AudioSettingCollection->Initialize(InLocalPlayer);

	// Volume
	{
		UGameSettingCollection* VolumeSettings = NewObject<UGameSettingCollection>();
		VolumeSettings->SetDevName(TEXT("VolumeCollection"));
		VolumeSettings->SetDisplayName(LOCTEXT("VolumeCollection_Name", "Volume"));
		AudioSettingCollection->AddSetting(VolumeSettings);

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

	return AudioSettingCollection;
}

UGameSettingCollection* UCL_GameSettingRegistry::InitializeUISettings(UCL_LocalPlayer* InLocalPlayer)
{
	UGameSettingCollection* UISettingCollection = NewObject<UGameSettingCollection>();
	UISettingCollection->SetDevName(TEXT("UICollection"));
	UISettingCollection->SetDisplayName(LOCTEXT("UICollection_Name", "UI"));
	UISettingCollection->Initialize(InLocalPlayer);

	// Minimap
	{
		UGameSettingCollection* MinimapSettings = NewObject<UGameSettingCollection>();
		MinimapSettings->SetDevName(TEXT("MinimapCollection"));
		MinimapSettings->SetDisplayName(LOCTEXT("MinimapCollection_Name", "Minimap"));
		UISettingCollection->AddSetting(MinimapSettings);

		// RotateMinimap
		{
			UGameSettingValueDiscreteDynamic_Bool* RotateMinimapSetting = NewObject<UGameSettingValueDiscreteDynamic_Bool>();
			RotateMinimapSetting->SetDevName(TEXT("RotateMinimap"));
			RotateMinimapSetting->SetDisplayName(LOCTEXT("RotateMinimap_Name", "Rotate Minimap"));
			RotateMinimapSetting->SetDescriptionRichText(LOCTEXT("RotateMinimap_Description", "Rotates the minimap with player view. Disable for a fixed map."));

			RotateMinimapSetting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetRotateMinimap));
			RotateMinimapSetting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetRotateMinimap));
			RotateMinimapSetting->SetDefaultValue(GetDefault<UCL_GameLocalUserSettings>()->GetRotateMinimap());

			MinimapSettings->AddSetting(RotateMinimapSetting);
		}
	}
	
	return UISettingCollection;
}

UGameSettingCollection* UCL_GameSettingRegistry::InitializeAccessibilitySettings(UCL_LocalPlayer* InLocalPlayer)
{
	UGameSettingCollection* AccessibilitySettingCollection = NewObject<UGameSettingCollection>();
	AccessibilitySettingCollection->SetDevName(TEXT("AccessibilityCollection"));
	AccessibilitySettingCollection->SetDisplayName(LOCTEXT("AccessibilityCollection_Name", "Accessibility"));
	AccessibilitySettingCollection->Initialize(InLocalPlayer);

	return AccessibilitySettingCollection;
}

//~ Begin UGameSettingRegistry

void UCL_GameSettingRegistry::SaveChanges()
{
	Super::SaveChanges();

	if (const UCL_LocalPlayer* LocalPlayer = Cast<UCL_LocalPlayer>(OwningLocalPlayer))
	{
		// Game user settings need to be applied to handle things like resolution, this saves indirectly
		LocalPlayer->GetLocalSettings()->ApplySettings(false);
	}
}

void UCL_GameSettingRegistry::OnInitialize(ULocalPlayer* InLocalPlayer)
{
	UCL_LocalPlayer* LocalPlayer = CastChecked<UCL_LocalPlayer>(InLocalPlayer);
	check(LocalPlayer);

	VideoSettings = InitializeVideoSettings(LocalPlayer);
	RegisterSetting(VideoSettings);
	
	GameSettings = InitializeGameSettings(LocalPlayer);
	RegisterSetting(GameSettings);
	
	AudioSettings = InitializeAudioSettings(LocalPlayer);
	RegisterSetting(AudioSettings);

	UISettings = InitializeUISettings(LocalPlayer);
	RegisterSetting(UISettings);

	AccessibilitySettings = InitializeAccessibilitySettings(LocalPlayer);
	RegisterSetting(AccessibilitySettings);
}

//~ End UGameSettingRegistry

#undef LOCTEXT_NAMESPACE
