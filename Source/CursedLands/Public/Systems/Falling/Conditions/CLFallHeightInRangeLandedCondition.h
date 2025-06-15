// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLLandedConditionBase.h"
#include "Engine/DataAsset.h"
#include "CLFallHeightInRangeLandedCondition.generated.h"

UENUM(Blueprintable)
enum class ECLFallHeightLandedCondition : uint8
{
	LessThan			UMETA(DisplayName = "Less Than", Description = "Fall Height must be less than the specified value"),
	InRange				UMETA(DisplayName = "In Range", Description = "Fall Height must be in the specified range"),
	GreaterThan			UMETA(DisplayName = "Greater Than", Description = "Fall Height must be greater than the specified value"),
};

UCLASS(EditInlineNew, Meta = (DisplayName = "Fall Height In Range"))
class CURSEDLANDS_API UCLFallHeightInRangeLandedCondition : public UCLLandedConditionBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
	ECLFallHeightLandedCondition Condition = ECLFallHeightLandedCondition::LessThan;
	
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true", EditCondition = "Condition != ECLFallHeightLandedCondition::InRange", EditConditionHides))
	float FallHeightThreshold;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true", EditCondition = "Condition == ECLFallHeightLandedCondition::InRange", EditConditionHides))
	float FallHeightThresholdMin;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true", EditCondition = "Condition == ECLFallHeightLandedCondition::InRange", EditConditionHides))
	float FallHeightThresholdMax;

	//~ UCLLandedConditionBase Begin
public:
	virtual bool TestCondition(const FCLLandedConditionContext& ConditionContext) const override;
	//~ UCLLandedConditionBase End
};
