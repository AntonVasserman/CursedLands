// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Components/CL_ExtendedCharacterMovementComponent.h"
#include "CL_SlidingCheckResult.generated.h"

USTRUCT()
struct FCL_SlidingCheckResult
{
	GENERATED_BODY()

	UPROPERTY()
	ECL_Stance SlideEndStance = ECL_Stance::Standing;

	UPROPERTY()
	FVector SlideEndLocation = FVector::ZeroVector;
};
