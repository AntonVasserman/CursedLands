// Copyright Anton Vasserman, All Rights Reserved.


#include "Settings/CL_GameLocalUserSettings.h"

UCL_GameLocalUserSettings* UCL_GameLocalUserSettings::Get()
{
	return GEngine ? CastChecked<UCL_GameLocalUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

float UCL_GameLocalUserSettings::GetOverallVolume() const
{
	return OverallVolume;
}

void UCL_GameLocalUserSettings::SetOverallVolume(float InVolume)
{
	OverallVolume = InVolume;
}
