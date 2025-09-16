// Copyright Anton Vasserman, All Rights Reserved.


#include "Settings/CL_GameSettingRegistry.h"

#include "GameSettingCollection.h"
#include "GameFramework/GameUserSettings.h"
#include "Player/CL_LocalPlayer.h"
#include "Settings/CL_GameLocalUserSettings.h"
#include "Settings/Collections/CL_AccessibilitySettingsCollection.h"
#include "Settings/Collections/CL_AudioSettingsCollection.h"
#include "Settings/Collections/CL_GameSettingsCollection.h"
#include "Settings/Collections/CL_UISettingsCollection.h"
#include "Settings/Collections/CL_VideoSettingsCollection.h"

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

	VideoSettings = NewObject<UCL_VideoSettingsCollection>();
	VideoSettings->InitializeSettings(LocalPlayer);
	RegisterSetting(VideoSettings);

	GameSettings = NewObject<UCL_GameSettingsCollection>();
	GameSettings->InitializeSettings(LocalPlayer);
	RegisterSetting(GameSettings);

	AudioSettings = NewObject<UCL_AudioSettingsCollection>();
	AudioSettings->InitializeSettings(LocalPlayer);
	RegisterSetting(AudioSettings);

	UISettings = NewObject<UCL_UISettingsCollection>();
	UISettings->InitializeSettings(LocalPlayer);
	RegisterSetting(UISettings);

	AccessibilitySettings = NewObject<UCL_AccessibilitySettingsCollection>();
	AccessibilitySettings->InitializeSettings(LocalPlayer);
	RegisterSetting(AccessibilitySettings);
}

//~ End UGameSettingRegistry

#undef LOCTEXT_NAMESPACE
