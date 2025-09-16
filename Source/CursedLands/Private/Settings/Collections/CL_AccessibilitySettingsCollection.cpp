// Copyright Anton Vasserman, All Rights Reserved.


#include "Settings/Collections/CL_AccessibilitySettingsCollection.h"

#include "GameSettingValueDiscreteDynamic.h"
#include "GameSettingValueScalarDynamic.h"
#include "DataSource/GameSettingDataSourceDynamic.h"
#include "EditCondition/WhenPlayingAsPrimaryPlayer.h"
#include "Player/CL_LocalPlayer.h"
#include "Settings/CL_GameLocalUserSettings.h"

#define LOCTEXT_NAMESPACE "CL"

//~ UCL_SettingsCollection Begin

void UCL_AccessibilitySettingsCollection::InitializeSettings(UCL_LocalPlayer* InLocalPlayer)
{
	SetDevName(TEXT("AccessibilityCollection"));
	SetDisplayName(LOCTEXT("AccessibilityCollection_Name", "Accessibility"));
	Initialize(InLocalPlayer);

	// Color Blind
	{
		UGameSettingCollection* ColorBlindSettings = NewObject<UGameSettingCollection>();
		ColorBlindSettings->SetDevName(TEXT("ColorBlindCollection"));
		ColorBlindSettings->SetDisplayName(LOCTEXT("ColorBlindCollection_Name", "ColorBlind"));
		AddSetting(ColorBlindSettings);

		// Color Blind Mode
		{
			UGameSettingValueDiscreteDynamic_Enum* ColorBlindSetting = NewObject<UGameSettingValueDiscreteDynamic_Enum>();
			ColorBlindSetting->SetDevName(TEXT("ColorBlindMode"));
			ColorBlindSetting->SetDisplayName(LOCTEXT("ColorBlindMode_Name", "Color Blind Mode"));
			ColorBlindSetting->SetDescriptionRichText(LOCTEXT("ColorBlindMode_Description", "Using the provided images, test out the different color blind modes to find a color correction that works best for you."));

			ColorBlindSetting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetColorBlindMode));
			ColorBlindSetting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetColorBlindMode));
			ColorBlindSetting->SetDefaultValue(GetDefault<UCL_GameLocalUserSettings>()->GetColorBlindMode());
			ColorBlindSetting->AddEnumOption(EColorBlindMode::Off, LOCTEXT("ColorBlindRotatorSettingOff", "Off"));
			ColorBlindSetting->AddEnumOption(EColorBlindMode::Deuteranope, LOCTEXT("ColorBlindRotatorSettingDeuteranope", "Deuteranope"));
			ColorBlindSetting->AddEnumOption(EColorBlindMode::Protanope, LOCTEXT("ColorBlindRotatorSettingProtanope", "Protanope"));
			ColorBlindSetting->AddEnumOption(EColorBlindMode::Tritanope, LOCTEXT("ColorBlindRotatorSettingTritanope", "Tritanope"));

			ColorBlindSetting->AddEditCondition(FWhenPlayingAsPrimaryPlayer::Get());

			ColorBlindSettings->AddSetting(ColorBlindSetting);
		}

		// Color Blind Strength
		{
			UGameSettingValueScalarDynamic* ColorBlindStrengthSetting = NewObject<UGameSettingValueScalarDynamic>();
			ColorBlindStrengthSetting->SetDevName(TEXT("ColorBlindStrength"));
			ColorBlindStrengthSetting->SetDisplayName(LOCTEXT("ColorBlindStrength_Name", "Color Blind Strength"));
			ColorBlindStrengthSetting->SetDescriptionRichText(LOCTEXT("ColorBlindStrength_Description", "Using the provided images, test out the different strengths to find a color correction that works best for you."));

			ColorBlindStrengthSetting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetColorBlindStrength));
			ColorBlindStrengthSetting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetColorBlindStrength));
			ColorBlindStrengthSetting->SetDefaultValue(GetDefault<UCL_GameLocalUserSettings>()->GetColorBlindStrength());
			ColorBlindStrengthSetting->SetDisplayFormat([](double SourceValue, double NormalizedValue)
				{
					return FText::Format(
						LOCTEXT("ColorBlindStrength", "{0}"),
						static_cast<int32>(FMath::GetMappedRangeValueClamped(FVector2D(0, 1), FVector2D(0, 10), NormalizedValue)));
				});

			ColorBlindStrengthSetting->SetSourceRangeAndStep(TRange<double>(0.0, 10.0), 1.0);

			ColorBlindStrengthSetting->AddEditCondition(FWhenPlayingAsPrimaryPlayer::Get());

			ColorBlindSettings->AddSetting(ColorBlindStrengthSetting);
		}
	}
}

//~ UCL_SettingsCollection End

#undef LOCTEXT_NAMESPACE
