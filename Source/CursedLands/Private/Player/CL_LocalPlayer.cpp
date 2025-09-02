// Copyright Anton Vasserman, All Rights Reserved.


#include "Player/CL_LocalPlayer.h"

#include "Settings/CL_GameLocalUserSettings.h"

UCL_LocalPlayer::UCL_LocalPlayer()
{
}

UCL_GameLocalUserSettings* UCL_LocalPlayer::GetLocalSettings() const
{
	return UCL_GameLocalUserSettings::Get();
}
