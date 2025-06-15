// Copyright Anton Vasserman, All Rights Reserved.

#include "Systems/Falling/Conditions/CLHitActorImplementsSoftLandingInterfaceLandedCondition.h"

#include "Systems/Falling/CLSoftLandingInterface.h"

bool UCLHitActorImplementsSoftLandingInterfaceLandedCondition::TestConditionInternal(
	const FCLLandedConditionContext& ConditionContext) const
{
	// If the Hit Actor isn't passed we don't fail the flow
	if (ConditionContext.HitActor == nullptr)
	{
		return false;
	}

	const bool bImplementsInterface = ConditionContext.HitActor->GetClass()->ImplementsInterface(UCLSoftLandingInterface::StaticClass());
	
	switch (ImplementsOption)
	{
	case ECLImplementsInterfaceOption::MustImplement:
		return bImplementsInterface;
	case ECLImplementsInterfaceOption::MustNotImplement:
		return !bImplementsInterface;
	default:
		// Should never reach here
		checkNoEntry();
		return false;
	}
}
