// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_TraversableActor.h"
#include "CL_TraversalAction.h"
#include "CL_TraversalCheckResult.generated.h"

USTRUCT(BlueprintType)
struct FCL_TraversalCheckResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	ECL_TraversalAction Action = ECL_TraversalAction::None;

	UPROPERTY(BlueprintReadOnly)
	FCL_LedgeCheckResult FrontLedgeCheckResult;

	UPROPERTY(BlueprintReadOnly)
	FCL_LedgeCheckResult BackLedgeCheckResult;

	UPROPERTY(BlueprintReadOnly)
	float BackLedgeHeight = 0.f;

	UPROPERTY(BlueprintReadOnly)
	bool bHasBackFloor = false;

	UPROPERTY(BlueprintReadOnly)
	FVector BackFloorLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	float BackFloorHeight = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float ObstacleHeight = 0.f;
	
	UPROPERTY(BlueprintReadOnly)
	float ObstacleDepth = 0.f;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPrimitiveComponent> HitComponent = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAnimMontage> ChosenMontage = nullptr;

	UPROPERTY(BlueprintReadOnly)
	float StartTime = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float PlayRate = 1.f;
};
