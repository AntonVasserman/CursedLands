// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/GameSettingScreen.h"

#include "CL_GameSettingScreen.generated.h"

UCLASS(Abstract, Meta = (Category = "Settings", DisableNativeTick))
class CURSEDLANDS_API UCL_GameSettingScreen : public UGameSettingScreen
{
	GENERATED_BODY()

	//~ Begin UGameSettingScreen
protected:
	virtual UGameSettingRegistry* CreateRegistry() override;
	//~ End UGameSettingScreen
};
