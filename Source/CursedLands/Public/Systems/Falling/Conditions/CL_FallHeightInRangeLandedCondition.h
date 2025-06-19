// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_LandedConditionBase.h"
#include "Engine/DataAsset.h"
#include "CL_FallHeightInRangeLandedCondition.generated.h"

UENUM(Blueprintable)
enum class ECL_FallHeightLandedCondition : uint8
{
	LessThan			UMETA(DisplayName = "Less Than", Description = "Fall Height must be less than the specified value"),
	InRange				UMETA(DisplayName = "In Range", Description = "Fall Height must be in the specified range"),
	GreaterThan			UMETA(DisplayName = "Greater Than", Description = "Fall Height must be greater than the specified value"),
};

UCLASS(EditInlineNew, Meta = (DisplayName = "Fall Height In Range"))
class CURSEDLANDS_API UCL_FallHeightInRangeLandedCondition : public UCL_LandedConditionBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (AllowPrivateAccess = "true"))
	ECL_FallHeightLandedCondition Condition = ECL_FallHeightLandedCondition::LessThan;
	
	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (AllowPrivateAccess = "true", EditCondition = "Condition != ECL_FallHeightLandedCondition::InRange", EditConditionHides))
	float FallHeightThreshold;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (AllowPrivateAccess = "true", EditCondition = "Condition == ECL_FallHeightLandedCondition::InRange", EditConditionHides))
	float FallHeightThresholdMin;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (AllowPrivateAccess = "true", EditCondition = "Condition == ECL_FallHeightLandedCondition::InRange", EditConditionHides))
	float FallHeightThresholdMax;

	//~ UCLLandedConditionBase Begin
protected:
	virtual bool TestConditionInternal_Implementation(const FCL_LandedConditionContext& ConditionContext) const override;
	//~ UCLLandedConditionBase End
};
