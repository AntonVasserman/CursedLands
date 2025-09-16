// Copyright Anton Vasserman, All Rights Reserved.


#include "Settings/Collections/CL_AccessibilitySettingsCollection.h"

#include "Player/CL_LocalPlayer.h"

#define LOCTEXT_NAMESPACE "CL"

//~ UCL_SettingsCollection Begin

void UCL_AccessibilitySettingsCollection::InitializeSettings(UCL_LocalPlayer* InLocalPlayer)
{
	SetDevName(TEXT("AccessibilityCollection"));
	SetDisplayName(LOCTEXT("AccessibilityCollection_Name", "Accessibility"));
	Initialize(InLocalPlayer);
}

//~ UCL_SettingsCollection End
