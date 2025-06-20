// Copyright Anton Vasserman, All Rights Reserved.

#include "Systems/Falling/Conditions/CL_FallingCondition.h"

bool FCL_FallingCondition::TestCondition(const FCL_FallingConditionContext& ConditionContext) const
{
	switch (ThresholdType)
	{
	case ECL_FallHeightThresholdType::LessThan:
		return ConditionContext.FallHeight < FallHeightThreshold;
	case ECL_FallHeightThresholdType::InRange:
		return ConditionContext.FallHeight >= FallHeightThresholdMin && ConditionContext.FallHeight <= FallHeightThresholdMax;
	case ECL_FallHeightThresholdType::GreaterThan:
		return ConditionContext.FallHeight > FallHeightThreshold;
	default:
		checkNoEntry();
		return false;
	}
}
