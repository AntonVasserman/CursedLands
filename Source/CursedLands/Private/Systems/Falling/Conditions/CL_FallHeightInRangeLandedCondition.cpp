// Copyright Anton Vasserman, All Rights Reserved.

#include "Systems/Falling/Conditions/CL_FallHeightInRangeLandedCondition.h"

bool UCL_FallHeightInRangeLandedCondition::TestConditionInternal_Implementation(const FCL_LandedConditionContext& ConditionContext) const
{
	switch (Condition)
	{
	case ECL_FallHeightLandedCondition::LessThan:
		return ConditionContext.FallHeight < FallHeightThreshold;
	case ECL_FallHeightLandedCondition::InRange:
		return ConditionContext.FallHeight >= FallHeightThresholdMin && ConditionContext.FallHeight <= FallHeightThresholdMax;
	case ECL_FallHeightLandedCondition::GreaterThan:
		return ConditionContext.FallHeight > FallHeightThreshold;
	default:
		checkNoEntry();
		return false;
	}
}
