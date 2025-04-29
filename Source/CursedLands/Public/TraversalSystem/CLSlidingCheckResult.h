// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CLCharacterMovementComponent.h"
#include "CLSlidingCheckResult.generated.h"

USTRUCT(BlueprintType)
struct FCLSlidingCheckResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	ECLStance SlideEndStance = ECLStance::Standing;

	UPROPERTY(BlueprintReadOnly)
	FVector SlideEndLocation = FVector::ZeroVector;
};
