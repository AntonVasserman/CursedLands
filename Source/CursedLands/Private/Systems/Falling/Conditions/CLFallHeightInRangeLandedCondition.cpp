// Copyright Anton Vasserman, All Rights Reserved.

#include "Systems/Falling/Conditions/CLFallHeightInRangeLandedCondition.h"

bool UCLFallHeightInRangeLandedCondition::TestCondition(const FCLLandedConditionContext& ConditionContext) const
{
	switch (Condition)
	{
	case ECLFallHeightLandedCondition::LessThan:
		return ConditionContext.FallHeight < FallHeightThreshold;
	case ECLFallHeightLandedCondition::InRange:
		return ConditionContext.FallHeight >= FallHeightThresholdMin && ConditionContext.FallHeight <= FallHeightThresholdMax;
	case ECLFallHeightLandedCondition::GreaterThan:
		return ConditionContext.FallHeight > FallHeightThreshold;
	default:
		checkNoEntry();
		return false;
	}
}
