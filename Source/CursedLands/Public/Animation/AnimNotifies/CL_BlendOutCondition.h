// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// TODO (CL-133): Make this "flags" so we can support multiple conditions
UENUM(BlueprintType)
enum class ECL_BlendOutCondition : uint8
{
    Always,
    IfMoving,
    IfFalling,
    IfFallingOrFlying,
};
