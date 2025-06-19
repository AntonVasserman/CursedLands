// Copyright Anton Vasserman, All Rights Reserved.

#include "Systems/Falling/Conditions/CL_LandedConditionBase.h"

bool UCL_LandedConditionBase::TestCondition(const FCL_LandedConditionContext& ConditionContext) const
{
	return (TestConditionInternal(ConditionContext) && (And == nullptr || And->TestCondition(ConditionContext)))
		|| (Or != nullptr && Or->TestCondition(ConditionContext));
}
