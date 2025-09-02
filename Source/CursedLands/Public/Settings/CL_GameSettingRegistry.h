// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameSettingRegistry.h"
#include "CL_GameSettingRegistry.generated.h"

#define GET_LOCAL_SETTINGS_FUNCTION_PATH(FunctionOrPropertyName)							\
	MakeShared<FGameSettingDataSourceDynamic>(TArray<FString>({								\
		GET_FUNCTION_NAME_STRING_CHECKED(UCL_LocalPlayer, GetLocalSettings),				\
		GET_FUNCTION_NAME_STRING_CHECKED(UCL_GameLocalUserSettings, FunctionOrPropertyName)	\
	}))

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
	TObjectPtr<UGameSettingCollection> GameSettings;

	/*
	UPROPERTY()
	TObjectPtr<UGameSettingCollection> HUDSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> AccessabilitySettings;
	*/
	
	UPROPERTY()
	TObjectPtr<UGameSettingCollection> AudioSettings;

	/*
	UPROPERTY()
	TObjectPtr<UGameSettingCollection> VideoSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> ControlsSettings;
	*/
	
	UGameSettingCollection* InitializeGameSettings(UCL_LocalPlayer* InLocalPlayer);
	UGameSettingCollection* InitializeAudioSettings(UCL_LocalPlayer* InLocalPlayer);
	
	//~ Begin UGameSettingRegistry
public:
	virtual void SaveChanges() override;
protected:
	virtual void OnInitialize(ULocalPlayer* InLocalPlayer) override;
	//~ End UGameSettingRegistry
};
