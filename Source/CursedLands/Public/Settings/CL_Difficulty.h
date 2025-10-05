// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class ECL_Difficulty : uint8
{
	Normal	UMETA(DisplayName = "Normal", Description = "Normal Difficulty"),
	Hard	UMETA(DisplayName = "Hard", Description = "Harder Difficulty"),
};
