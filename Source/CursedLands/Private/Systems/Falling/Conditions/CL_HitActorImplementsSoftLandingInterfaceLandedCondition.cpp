// Copyright Anton Vasserman, All Rights Reserved.

#include "Systems/Falling/Conditions/CL_HitActorImplementsSoftLandingInterfaceLandedCondition.h"

#include "Systems/Falling/CL_SoftLandingInterface.h"

bool UCL_HitActorImplementsSoftLandingInterfaceLandedCondition::TestConditionInternal_Implementation(
	const FCL_LandedConditionContext& ConditionContext) const
{
	// If the Hit Actor isn't passed we don't fail the flow
	if (ConditionContext.HitActor == nullptr)
	{
		return false;
	}

	const bool bImplementsInterface = ConditionContext.HitActor->GetClass()->ImplementsInterface(UCL_SoftLandingInterface::StaticClass());
	
	switch (ImplementsOption)
	{
	case ECL_ImplementsInterfaceOption::MustImplement:
		return bImplementsInterface;
	case ECL_ImplementsInterfaceOption::MustNotImplement:
		return !bImplementsInterface;
	default:
		// Should never reach here
		checkNoEntry();
		return false;
	}
}
