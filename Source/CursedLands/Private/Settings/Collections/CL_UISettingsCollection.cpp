// Copyright Anton Vasserman, All Rights Reserved.


#include "Settings/Collections/CL_UISettingsCollection.h"

#include "GameSettingValueDiscreteDynamic.h"
#include "DataSource/GameSettingDataSourceDynamic.h"
#include "Player/CL_LocalPlayer.h"
#include "Settings/CL_GameLocalUserSettings.h"

#define LOCTEXT_NAMESPACE "CL"

//~ UCL_SettingsCollection Begin

void UCL_UISettingsCollection::InitializeSettings(UCL_LocalPlayer* InLocalPlayer)
{
	SetDevName(TEXT("UICollection"));
	SetDisplayName(LOCTEXT("UICollection_Name", "UI"));
	Initialize(InLocalPlayer);

	// Minimap
	{
		UGameSettingCollection* MinimapSettings = NewObject<UGameSettingCollection>();
		MinimapSettings->SetDevName(TEXT("MinimapCollection"));
		MinimapSettings->SetDisplayName(LOCTEXT("MinimapCollection_Name", "Minimap"));
		AddSetting(MinimapSettings);

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
}

//~ UCL_SettingsCollection End

#undef LOCTEXT_NAMESPACE
