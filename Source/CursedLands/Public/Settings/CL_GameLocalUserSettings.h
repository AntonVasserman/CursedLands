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

UENUM(BlueprintType)
enum class EColorBlindMode : uint8
{
	Off,
	Deuteranope	UMETA(Description = "Deuteranope (green weak/blind)"),
	Protanope	UMETA(Description = "Protanope (red weak/blind)"),
	Tritanope	UMETA(Description = "Tritanope (blue weak/blind)")
};

UCLASS()
class CURSEDLANDS_API UCL_GameLocalUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UCL_GameLocalUserSettings();
	
	static UCL_GameLocalUserSettings* Get();

	// Begin Video Settings
public:
	UFUNCTION()
	float GetDisplayGamma() const;
	UFUNCTION()
	void SetDisplayGamma(float InGamma);
	
private:
	void ApplyDisplayGamma();
	
	UPROPERTY(Config)
	float DisplayGamma = 2.2;
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
	float GetMasterVolume() const;
	UFUNCTION()
	void SetMasterVolume(float InVolume);

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
	float MasterVolume = 1.0f;
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
public:
	UFUNCTION()
	EColorBlindMode GetColorBlindMode() const;
	UFUNCTION()
	void SetColorBlindMode(EColorBlindMode InMode);

	UFUNCTION()
	int32 GetColorBlindStrength() const;
	UFUNCTION()
	void SetColorBlindStrength(int32 InColorBlindStrength);

private:
	UPROPERTY(Config)
	EColorBlindMode ColorBlindMode = EColorBlindMode::Off;

	UPROPERTY(Config)
	int32 ColorBlindStrength = 10;

	// End Accessibility Settings

	//~ UGameUserSettings Begin
public:
	virtual void SetToDefaults() override;
	virtual void ApplyNonResolutionSettings() override;
	//~ UGameUserSettings End
};
