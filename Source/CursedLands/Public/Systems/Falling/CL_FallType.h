// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECL_FallType : uint8
{
	Light		UMETA(DisplayName = "Light"),
	Medium		UMETA(DisplayName = "Medium"),
	Deadly	    UMETA(DisplayName = "Deadly"),
};