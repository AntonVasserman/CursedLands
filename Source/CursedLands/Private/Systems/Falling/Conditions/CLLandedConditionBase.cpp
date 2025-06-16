// Copyright Anton Vasserman, All Rights Reserved.

#include "Systems/Falling/Conditions/CLLandedConditionBase.h"

bool UCLLandedConditionBase::TestCondition(const FCLLandedConditionContext& ConditionContext) const
{
	return (TestConditionInternal(ConditionContext) && (And == nullptr || And->TestCondition(ConditionContext)))
		|| (Or != nullptr && Or->TestCondition(ConditionContext));
}
