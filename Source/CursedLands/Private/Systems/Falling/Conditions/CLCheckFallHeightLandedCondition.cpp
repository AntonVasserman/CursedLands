// Copyright Anton Vasserman, All Rights Reserved.


#include "Systems/Falling/Conditions/CLCheckFallHeightLandedCondition.h"

bool UCLCheckFallHeightLandedCondition::TestCondition(float FallHeight) const
{
	switch (Condition)
	{
	case ECLFallHeightLandedCondition::LessThan:
		return FallHeight < FallHeightThreshold;
	case ECLFallHeightLandedCondition::InRange:
		return FallHeight >= FallHeightThresholdMin && FallHeight <= FallHeightThresholdMax;
	case ECLFallHeightLandedCondition::GreaterThan:
		return FallHeight > FallHeightThreshold;
	default:
		checkNoEntry();
		return false;
	}
}
