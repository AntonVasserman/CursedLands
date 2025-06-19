// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CL_SoftLandingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UCL_SoftLandingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * @class ICL_SoftLandingInterface
 *
 * @brief Interface representing Actors that if landed on, the Player will not receive damage.
 */
class CURSEDLANDS_API ICL_SoftLandingInterface
{
	GENERATED_BODY()
};
