// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLTraversalCheckInput.generated.h"

USTRUCT(BlueprintType)
struct FCLTraversalCheckInput
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	FVector TraceForwardDirection = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	float TraceForwardDistance = 0.f;

	UPROPERTY(BlueprintReadOnly)
	FVector TraceStartOffset = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	FVector TraceEndOffset = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	float TraceRadius = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float TraceHalfHeight = 0.f;
};
