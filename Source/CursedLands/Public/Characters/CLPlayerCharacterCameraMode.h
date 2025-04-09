// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class ECLPlayerCharacterCameraMode : uint8
{
	Default	UMETA(DisplayName = "Default", Description = "Default camera mode"),
	Close	UMETA(DisplayName = "Close", Description = "Close to Character camera mode"),
};
