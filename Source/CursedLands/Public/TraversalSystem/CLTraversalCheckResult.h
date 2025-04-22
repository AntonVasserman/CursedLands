// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLTraversalAction.h"
#include "CLTraversalCheckResult.generated.h"

USTRUCT(BlueprintType)
struct FCLTraversalCheckResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	ECLTraversalAction Action = ECLTraversalAction::None;

	UPROPERTY(BlueprintReadOnly)
	bool bHasFrontLedge = false;

	UPROPERTY(BlueprintReadOnly)
	FVector FrontLedgeLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	FVector FrontLedgeNormal = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	bool bHasBackLedge = false;

	UPROPERTY(BlueprintReadOnly)
	FVector BackLedgeLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	FVector BackLedgeNormal = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	float BackLedgeHeight = 0.f;

	UPROPERTY(BlueprintReadOnly)
	bool bHasBackFloor = false;

	UPROPERTY(BlueprintReadOnly)
	FVector BackFloorLocation = FVector::ZeroVector;

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
