// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameSettingCollection.h"
#include "CL_SettingsCollection.generated.h"

class UCL_LocalPlayer;

#define GET_LOCAL_SETTINGS_FUNCTION_PATH(FunctionOrPropertyName)							\
	MakeShared<FGameSettingDataSourceDynamic>(TArray<FString>({								\
		GET_FUNCTION_NAME_STRING_CHECKED(UCL_LocalPlayer, GetLocalSettings),				\
		GET_FUNCTION_NAME_STRING_CHECKED(UCL_GameLocalUserSettings, FunctionOrPropertyName)	\
}))

UCLASS(Abstract)
class CURSEDLANDS_API UCL_SettingsCollection : public UGameSettingCollection
{
	GENERATED_BODY()

public:
	virtual void InitializeSettings(UCL_LocalPlayer* InLocalPlayer) { unimplemented() }
};
