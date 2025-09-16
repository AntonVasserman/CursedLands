// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameSettingRegistry.h"
#include "CL_GameSettingRegistry.generated.h"

class UCL_AudioSettingsCollection;
class UCL_GameSettingsCollection;
class UCL_AccessibilitySettingsCollection;
class UCL_VideoSettingsCollection;
class UCL_UISettingsCollection;
class ULocalPlayer;
class UObject;
class UGameSettingCollection;
class UCL_LocalPlayer;

UCLASS()
class CURSEDLANDS_API UCL_GameSettingRegistry : public UGameSettingRegistry
{
	GENERATED_BODY()

public:
	static UCL_GameSettingRegistry* Get(UCL_LocalPlayer* InLocalPlayer);

protected:
	UPROPERTY()
	TObjectPtr<UCL_VideoSettingsCollection> VideoSettings;

	UPROPERTY()
	TObjectPtr<UCL_GameSettingsCollection> GameSettings;
	
	UPROPERTY()
	TObjectPtr<UCL_AudioSettingsCollection> AudioSettings;

	UPROPERTY()
	TObjectPtr<UCL_UISettingsCollection> UISettings;
	
	UPROPERTY()
	TObjectPtr<UCL_AccessibilitySettingsCollection> AccessibilitySettings;
	
	//~ Begin UGameSettingRegistry
public:
	virtual void SaveChanges() override;
protected:
	virtual void OnInitialize(ULocalPlayer* InLocalPlayer) override;
	//~ End UGameSettingRegistry
};
