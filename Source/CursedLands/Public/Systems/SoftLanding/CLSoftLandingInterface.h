// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CLSoftLandingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UCLSoftLandingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * @class ICLSoftLandingInterface
 *
 * @brief Interface representing Actors that if landed on, the Player will not receive damage.
 */
class CURSEDLANDS_API ICLSoftLandingInterface
{
	GENERATED_BODY()
};
