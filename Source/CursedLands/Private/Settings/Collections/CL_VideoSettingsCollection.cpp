// Copyright Anton Vasserman, All Rights Reserved.


#include "Settings/Collections/CL_VideoSettingsCollection.h"

#include "GameSettingValueDiscreteDynamic.h"
#include "GameSettingValueScalarDynamic.h"
#include "DataSource/GameSettingDataSourceDynamic.h"
#include "EditCondition/WhenPlayingAsPrimaryPlayer.h"
#include "Player/CL_LocalPlayer.h"
#include "Settings/CL_GameLocalUserSettings.h"

#define LOCTEXT_NAMESPACE "CL"

//~ UCL_SettingsCollection Begin

void UCL_VideoSettingsCollection::InitializeSettings(UCL_LocalPlayer* InLocalPlayer)
{
	SetDevName(TEXT("VideoCollection"));
	SetDisplayName(LOCTEXT("VideoCollection_Name", "Video"));
	Initialize(InLocalPlayer);

	// Display
	{
		UGameSettingCollection* DisplaySettings = NewObject<UGameSettingCollection>();
		DisplaySettings->SetDevName(TEXT("DisplayCollection"));
		DisplaySettings->SetDisplayName(LOCTEXT("DisplayCollection_Name", "Display"));
		AddSetting(DisplaySettings);

		// WindowMode
		{
			UGameSettingValueDiscreteDynamic_Enum* WindowModeSetting = NewObject<UGameSettingValueDiscreteDynamic_Enum>();
			WindowModeSetting->SetDevName(TEXT("WindowMode"));
			WindowModeSetting->SetDisplayName(LOCTEXT("WindowMode_Name", "Window Mode"));
			WindowModeSetting->SetDescriptionRichText(LOCTEXT("WindowMode_Description", "Switch between windowed and fullscreen mode."));

			WindowModeSetting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetFullscreenMode));
			WindowModeSetting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetFullscreenModeAndApply));
			WindowModeSetting->AddEnumOption(EWindowMode::Fullscreen, LOCTEXT("WindowModeFullscreen", "Fullscreen"));
			WindowModeSetting->AddEnumOption(EWindowMode::WindowedFullscreen, LOCTEXT("WindowModeWindowedFullscreen", "Windowed Fullscreen"));
			WindowModeSetting->AddEnumOption(EWindowMode::Windowed, LOCTEXT("WindowModeWindowed", "Windowed"));

			DisplaySettings->AddSetting(WindowModeSetting);
		}
	}

	// Graphics
	{
		UGameSettingCollection* GraphicsSettings = NewObject<UGameSettingCollection>();
		GraphicsSettings->SetDevName(TEXT("GraphicsCollection"));
		GraphicsSettings->SetDisplayName(LOCTEXT("GraphicsCollection_Name", "Graphics"));
		AddSetting(GraphicsSettings);

		// Brightness
		{
			UGameSettingValueScalarDynamic* BrightnessSetting = NewObject<UGameSettingValueScalarDynamic>();
			BrightnessSetting->SetDevName(TEXT("Brightness"));
			BrightnessSetting->SetDisplayName(LOCTEXT("Brightness_Name", "Brightness"));
			BrightnessSetting->SetDescriptionRichText(LOCTEXT("Brightness_Description", "Adjusts the brightness."));

			BrightnessSetting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetDisplayGamma));
			BrightnessSetting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetDisplayGamma));
			BrightnessSetting->SetDefaultValue(2.2);
			BrightnessSetting->SetDisplayFormat([](double SourceValue, double NormalizedValue)
				{
					return FText::Format(
						LOCTEXT("BrightnessFormat", "{0}%"),
						static_cast<int32>(FMath::GetMappedRangeValueClamped(FVector2D(0, 1), FVector2D(50, 150), NormalizedValue)));
				});
			BrightnessSetting->SetSourceRangeAndStep(TRange<double>(1.7, 2.7), 0.01);

			BrightnessSetting->AddEditCondition(FWhenPlayingAsPrimaryPlayer::Get());

			GraphicsSettings->AddSetting(BrightnessSetting);
		}
	}
}

//~ UCL_SettingsCollection End

#undef LOCTEXT_NAMESPACE
