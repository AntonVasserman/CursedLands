// Copyright Anton Vasserman, All Rights Reserved.


#include "Settings/CL_GameSettingRegistry.h"

#include "GameSettingCollection.h"
#include "GameSettingValueScalarDynamic.h"
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

UGameSettingCollection* UCL_GameSettingRegistry::InitializeGameSettings(UCL_LocalPlayer* InLocalPlayer)
{
	UGameSettingCollection* Screen = NewObject<UGameSettingCollection>();
	Screen->SetDevName(TEXT("GameCollection"));
	Screen->SetDisplayName(LOCTEXT("GameCollection_Name", "Game"));
	Screen->Initialize(InLocalPlayer);

	// TODO

	return Screen;
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
		
		{
			UGameSettingValueScalarDynamic* OverallVolumeSetting = NewObject<UGameSettingValueScalarDynamic>();
			OverallVolumeSetting->SetDevName(TEXT("OverallVolume"));
			OverallVolumeSetting->SetDisplayName(LOCTEXT("OverallVolume_Name", "Overall"));
			OverallVolumeSetting->SetDescriptionRichText(LOCTEXT("OverallVolume_Description", "Adjusts the volume of everything."));

			OverallVolumeSetting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetOverallVolume));
			OverallVolumeSetting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetOverallVolume));
			OverallVolumeSetting->SetDefaultValue(GetDefault<UCL_GameLocalUserSettings>()->GetOverallVolume());
			OverallVolumeSetting->SetDisplayFormat(UGameSettingValueScalarDynamic::ZeroToOnePercent);

			OverallVolumeSetting->AddEditCondition(FWhenPlayingAsPrimaryPlayer::Get());

			VolumeSettings->AddSetting(OverallVolumeSetting);
		}
	}

	return AudioSettingCollection;
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

	GameSettings = InitializeGameSettings(LocalPlayer);
	RegisterSetting(GameSettings);
	
	AudioSettings = InitializeAudioSettings(LocalPlayer);
	RegisterSetting(AudioSettings);
}

//~ End UGameSettingRegistry

#undef LOCTEXT_NAMESPACE
