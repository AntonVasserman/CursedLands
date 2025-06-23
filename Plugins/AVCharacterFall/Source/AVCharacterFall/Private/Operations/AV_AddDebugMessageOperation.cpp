// Copyright Anton Vasserman, All Rights Reserved.

#include "Operations/AV_AddDebugMessageOperation.h"

#include "Components/AV_CharacterFallComponent.h"
#include "Engine/Engine.h"

void FAV_AddDebugMessageOperation::Execute() const
{
	if (bInLog)
	{
		UE_LOG(LogAV_CharacterFall, Display, TEXT("%s"), *Message);
	}
	
	if (bOnScreen)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, MessageColor, *Message);
	}
}
