// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_FallHeightThresholdType.h"
#include "UObject/Object.h"
#include "CL_FallingCondition.generated.h"

USTRUCT(BlueprintType)
struct CURSEDLANDS_API FCL_FallingConditionContext
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACharacter> Character;

	UPROPERTY(BlueprintReadOnly)
	float FallHeight = 0.f;
};

USTRUCT(Blueprintable, Meta = (DisplayName = "Falling Condition"))
struct CURSEDLANDS_API FCL_FallingCondition
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (AllowPrivateAccess = "true"))
	ECL_FallHeightThresholdType ThresholdType = ECL_FallHeightThresholdType::LessThan;
	
	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (AllowPrivateAccess = "true", EditCondition = "ThresholdType != ECL_FallHeightThresholdType::InRange", EditConditionHides))
	float FallHeightThreshold;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (AllowPrivateAccess = "true", EditCondition = "ThresholdType == ECL_FallHeightThresholdType::InRange", EditConditionHides))
	float FallHeightThresholdMin;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (AllowPrivateAccess = "true", EditCondition = "ThresholdType == ECL_FallHeightThresholdType::InRange", EditConditionHides))
	float FallHeightThresholdMax;

public:
	bool TestCondition(const FCL_FallingConditionContext& ConditionContext) const;
};