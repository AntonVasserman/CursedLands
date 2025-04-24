// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLTraversableActor.h"
#include "CLTraversalAction.h"
#include "CLTraversalCheckResult.generated.h"

USTRUCT(BlueprintType)
struct FCLTraversalCheckResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	ECLTraversalAction Action = ECLTraversalAction::None;

	UPROPERTY(BlueprintReadOnly)
	FLedgeCheckResult FrontLedgeCheckResult;

	UPROPERTY(BlueprintReadOnly)
	FLedgeCheckResult BackLedgeCheckResult;

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
