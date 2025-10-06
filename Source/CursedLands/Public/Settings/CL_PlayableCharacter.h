// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class ECL_PlayableCharacter : uint8
{
	Manny	UMETA(DisplayName = "UE Manny", Description = "Unreal Engine's Manny"),
	XBot	UMETA(DisplayName = "Mixamo XBot", Description = "Mixamo's Xbot"),
	YBot	UMETA(DisplayName = "Mixamo YBot", Description = "Mixamo's YBot"),
	Gideon	UMETA(DisplayName = "Paragon Gideon", Description = "Paragon's Gideon"),
	Shinbi	UMETA(DisplayName = "Paragon Shinbi", Description = "Paragon's Shinbi"),
};
