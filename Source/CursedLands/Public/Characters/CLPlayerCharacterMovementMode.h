// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class ECLPlayerCharacterMovementMode : uint8
{
	Default		UMETA(DisplayName = "Default", Description = "Default movement mode, used when not targeting a target outside of combat"),
	Strafing	UMETA(DisplayName = "Strafing", Description = "Strafing movement mode, used when targeting a target in combat"),
};
