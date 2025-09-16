// Copyright Anton Vasserman, All Rights Reserved.


#include "Settings/Collections/CL_GameSettingsCollection.h"

#include "GameSettingValueDiscreteDynamic.h"
#include "DataSource/GameSettingDataSourceDynamic.h"
#include "Player/CL_LocalPlayer.h"
#include "Settings/CL_GameLocalUserSettings.h"

#define LOCTEXT_NAMESPACE "CL"

//~ UCL_SettingsCollection Begin

void UCL_GameSettingsCollection::InitializeSettings(UCL_LocalPlayer* InLocalPlayer)
{
	SetDevName(TEXT("GameCollection"));
	SetDisplayName(LOCTEXT("GameCollection_Name", "Game"));
	Initialize(InLocalPlayer);
	
	// Camera
	{ 
		UGameSettingCollection* CameraSettings = NewObject<UGameSettingCollection>();
		CameraSettings->SetDevName(TEXT("CameraCollection"));
		CameraSettings->SetDisplayName(LOCTEXT("CameraCollection_Name", "Camera"));
		AddSetting(CameraSettings);

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
}

//~ UCL_SettingsCollection End
