// Copyright Anton Vasserman, All Rights Reserved.


#include "UI/Settings/CL_GameSettingScreen.h"

#include "Player/CL_LocalPlayer.h"
#include "Settings/CL_GameSettingRegistry.h"

UGameSettingRegistry* UCL_GameSettingScreen::CreateRegistry()
{
	UCL_GameSettingRegistry* NewRegistry = NewObject<UCL_GameSettingRegistry>();

	if (UCL_LocalPlayer* LocalPlayer = CastChecked<UCL_LocalPlayer>(GetOwningLocalPlayer()))
	{
		NewRegistry->Initialize(LocalPlayer);
	}

	return NewRegistry;
}
