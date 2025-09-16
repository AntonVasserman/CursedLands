// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SoundControlBus.h"
#include "Characters/CL_PlayerCharacterCameraMode.h"
#include "GameFramework/GameUserSettings.h"
#include "CL_GameLocalUserSettings.generated.h"

class USoundControlBusMix;
enum class ECL_PlayerCharacterCameraMode : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCL_OnRotateMinimapChanged, bool, bNewRotateMinimap);

UCLASS()
class CURSEDLANDS_API UCL_GameLocalUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UCL_GameLocalUserSettings();
	
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
	ECL_PlayerCharacterCameraMode CameraDistance = ECL_PlayerCharacterCameraMode::Default;
	// End Game Settings
	
	// Begin Audio Settings
public:
	UFUNCTION()
	float GetOverallVolume() const;
	UFUNCTION()
	void SetOverallVolume(float InVolume);

	UFUNCTION()
	float GetMusicVolume() const;
	UFUNCTION()
	void SetMusicVolume(float InVolume);

	UFUNCTION()
	float GetSFXVolume() const;
	UFUNCTION()
	void SetSFXVolume(float InVolume);

	UFUNCTION()
	float GetVoiceVolume() const;
	UFUNCTION()
	void SetVoiceVolume(float InVolume);

private:
	void LoadUserControlBusMix();
	void SetVolumeForControlBus(USoundControlBus* InSoundControlBus, float InVolume);
	
	UPROPERTY(Config)
	float OverallVolume = 1.0f;
	UPROPERTY(Config)
	float MusicVolume = 1.0f;
	UPROPERTY(Config)
	float SFXVolume = 1.0f;
	UPROPERTY(Config)
	float VoiceVolume = 1.0f;

	UPROPERTY(Transient)
	TMap<FName, TObjectPtr<USoundControlBus>> ControlBusMap;

	UPROPERTY(Transient)
	TObjectPtr<USoundControlBusMix> ControlBusMix = nullptr;
	
	UPROPERTY(Transient)
	bool bSoundControlBusMixLoaded;
	// End Audio Settings

	// Begin UI Settings
public:
	FCL_OnRotateMinimapChanged OnRotateMinimapChanged;
	UFUNCTION()
	bool GetRotateMinimap() const;
	UFUNCTION()
	void SetRotateMinimap(bool bInRotateMinimap);
	
private:
	UPROPERTY(Config)
	bool bRotateMinimap = true;
	// End UI Settings
	
	// Begin Accessibility Settings
	// End Accessibility Settings

	//~ UGameUserSettings Begin
public:
	virtual void SetToDefaults() override;
	virtual void ApplyNonResolutionSettings() override;
	//~ UGameUserSettings End
};
