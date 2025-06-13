// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CLCheckFallHeightLandedCondition.generated.h"

UENUM(Blueprintable)
enum class ECLFallHeightLandedCondition : uint8
{
	LessThan			UMETA(DisplayName = "Less Than", Description = "Fall Height must be less than the specified value"),
	InRange				UMETA(DisplayName = "In Range", Description = "Fall Height must be in the specified range"),
	GreaterThan			UMETA(DisplayName = "Greater Than", Description = "Fall Height must be greater than the specified value"),
};

USTRUCT()
struct CURSEDLANDS_API FCLCheckFallHeightLandedCondition
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	ECLFallHeightLandedCondition Condition = ECLFallHeightLandedCondition::LessThan;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (AllowPrivateAccess = "true", EditCondition = "Condition != ECLFallHeightLandedCondition::InRange", EditConditionHides))
	float FallHeightThreshold;

	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (AllowPrivateAccess = "true", EditCondition = "Condition == ECLFallHeightLandedCondition::InRange", EditConditionHides))
	float FallHeightThresholdMin;

	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (AllowPrivateAccess = "true", EditCondition = "Condition == ECLFallHeightLandedCondition::InRange", EditConditionHides))
	float FallHeightThresholdMax;
	
public:
	bool TestCondition(float FallHeight) const;
};
