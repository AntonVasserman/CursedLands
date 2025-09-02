// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "CL_GameLocalUserSettings.generated.h"

enum class ECL_PlayerCharacterCameraMode : uint8;

UCLASS()
class CURSEDLANDS_API UCL_GameLocalUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	static UCL_GameLocalUserSettings* Get();

	// Begin Video Settings
	// End Video Settings
	
	// Begin Game Settings
public:
	UFUNCTION()
	ECL_PlayerCharacterCameraMode GetCameraDistance() const;
	UFUNCTION()
	void SetCameraDistance(ECL_PlayerCharacterCameraMode InCameraDistance);
	
private:
	UPROPERTY(Config)
	ECL_PlayerCharacterCameraMode CameraDistance;
	// End Game Settings
	
	// Begin Audio Settings
public:
	UFUNCTION()
	float GetMasterVolume() const;
	UFUNCTION()
	void SetMasterVolume(float InVolume);

	UFUNCTION()
	float GetMusicVolume() const;
	UFUNCTION()
	void SetMusicVolume(float InVolume);

	UFUNCTION()
	float GetEffectsVolume() const;
	UFUNCTION()
	void SetEffectsVolume(float InVolume);

	UFUNCTION()
	float GetVoiceVolume() const;
	UFUNCTION()
	void SetVoiceVolume(float InVolume);

private:
	UPROPERTY(Config)
	float MasterVolume = 1.0f;
	UPROPERTY(Config)
	float MusicVolume = 1.0f;
	UPROPERTY(Config)
	float EffectsVolume = 1.0f;
	UPROPERTY(Config)
	float VoiceVolume = 1.0f;
	// End Audio Settings

	// Begin UI Settings
public:
	UFUNCTION()
	bool GetRotateMinimap() const;
	UFUNCTION()
	void SetRotateMinimap(bool bInRotateMinimap);
	
private:
	UPROPERTY(Config)
	bool bRotateMinimap;
	// End UI Settings
	
	// Begin Accessibility Settings
	// End Accessibility Settings
};
