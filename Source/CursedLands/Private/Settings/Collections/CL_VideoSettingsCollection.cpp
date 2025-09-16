// Copyright Anton Vasserman, All Rights Reserved.


#include "Settings/Collections/CL_VideoSettingsCollection.h"

#include "Player/CL_LocalPlayer.h"

#define LOCTEXT_NAMESPACE "CL"

//~ UCL_SettingsCollection Begin

void UCL_VideoSettingsCollection::InitializeSettings(UCL_LocalPlayer* InLocalPlayer)
{
	SetDevName(TEXT("VideoCollection"));
	SetDisplayName(LOCTEXT("VideoCollection_Name", "Video"));
	Initialize(InLocalPlayer);
}

//~ UCL_SettingsCollection End

#undef LOCTEXT_NAMESPACE
