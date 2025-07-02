// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_TraversableActor.h"
#include "CL_TraversalAction.h"
#include "CL_TraversalCheckResult.generated.h"

USTRUCT()
struct FCL_TraversalCheckResult
{
	GENERATED_BODY()

	UPROPERTY()
	ECL_TraversalAction Action = ECL_TraversalAction::None;

	UPROPERTY()
	FCL_LedgeCheckResult FrontLedgeCheckResult;

	UPROPERTY()
	FCL_LedgeCheckResult BackLedgeCheckResult;

	UPROPERTY()
	float BackLedgeHeight = 0.f;

	UPROPERTY()
	bool bHasBackFloor = false;

	UPROPERTY()
	FVector BackFloorLocation = FVector::ZeroVector;

	UPROPERTY()
	float BackFloorHeight = 0.f;

	UPROPERTY()
	float ObstacleHeight = 0.f;
	
	UPROPERTY()
	float ObstacleDepth = 0.f;

	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> HitComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UAnimMontage> ChosenMontage = nullptr;

	UPROPERTY()
	float StartTime = 0.f;

	UPROPERTY()
	float PlayRate = 1.f;
};
