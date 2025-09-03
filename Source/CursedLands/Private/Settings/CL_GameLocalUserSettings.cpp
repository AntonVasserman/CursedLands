// Copyright Anton Vasserman, All Rights Reserved.


#include "Settings/CL_GameLocalUserSettings.h"

#include "Characters/CL_PlayerCharacterCameraMode.h"

UCL_GameLocalUserSettings* UCL_GameLocalUserSettings::Get()
{
	return GEngine ? CastChecked<UCL_GameLocalUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

// Begin Video Settings
// End Video Settings

// Begin Game Settings
ECL_PlayerCharacterCameraMode UCL_GameLocalUserSettings::GetCameraDistance() const
{
	return CameraDistance;
}

void UCL_GameLocalUserSettings::SetCameraDistance(ECL_PlayerCharacterCameraMode InCameraDistance)
{
	CameraDistance = InCameraDistance;
}
// End Game Settings

// Begin Audio Settings
float UCL_GameLocalUserSettings::GetMasterVolume() const
{
	return MasterVolume;
}

void UCL_GameLocalUserSettings::SetMasterVolume(float InVolume)
{
	MasterVolume = InVolume;
}

float UCL_GameLocalUserSettings::GetMusicVolume() const
{
	return MusicVolume;
}

void UCL_GameLocalUserSettings::SetMusicVolume(float InVolume)
{
	MusicVolume = InVolume;
}

float UCL_GameLocalUserSettings::GetEffectsVolume() const
{
	return EffectsVolume;
}

void UCL_GameLocalUserSettings::SetEffectsVolume(float InVolume)
{
	EffectsVolume = InVolume;
}

float UCL_GameLocalUserSettings::GetVoiceVolume() const
{
	return VoiceVolume;
}

void UCL_GameLocalUserSettings::SetVoiceVolume(float InVolume)
{
	VoiceVolume = InVolume;
}
// End Audio Settings

// Begin UI Settings
bool UCL_GameLocalUserSettings::GetRotateMinimap() const
{
	return bRotateMinimap;
}

void UCL_GameLocalUserSettings::SetRotateMinimap(bool bInRotateMinimap)
{
	bRotateMinimap = bInRotateMinimap;
	OnRotateMinimapChanged.Broadcast(bRotateMinimap);
}
// End UI Settings

// Begin Accessibility Settings
// End Accessibility Settings
