// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CL_AudioSettings.generated.h"

UCLASS(Config = "Game", DefaultConfig, Meta = (DisplayName = "Audio Settings"))
class CURSEDLANDS_API UCL_AudioSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "UserMixSettings", Meta = (AllowedClasses = "/Script/AudioModulation.SoundControlBusMix"))
	FSoftObjectPath UserSettingsControlBusMix;
	
	UPROPERTY(Config, EditAnywhere, Category = "UserMixSettings", Meta = (AllowedClasses = "/Script/AudioModulation.SoundControlBus"))
	FSoftObjectPath MasterVolumeControlBus;

	UPROPERTY(Config, EditAnywhere, Category = "UserMixSettings", Meta = (AllowedClasses = "/Script/AudioModulation.SoundControlBus"))
	FSoftObjectPath MusicVolumeControlBus;
	
	UPROPERTY(Config, EditAnywhere, Category = "UserMixSettings", DisplayName="SFX Volume Control Bus", Meta = (AllowedClasses = "/Script/AudioModulation.SoundControlBus"))
	FSoftObjectPath SFXVolumeControlBus;

	UPROPERTY(Config, EditAnywhere, Category = "UserMixSettings", Meta = (AllowedClasses = "/Script/AudioModulation.SoundControlBus"))
	FSoftObjectPath VoiceVolumeControlBus;
};
