// Copyright Anton Vasserman, All Rights Reserved.

#include "Systems/Falling/Tasks/Landed/Constraints/CL_HitActorImplementsSoftLandingInterfaceLandedConstraint.h"

#include "Systems/Falling/CL_SoftLandingInterface.h"

bool UCL_HitActorImplementsSoftLandingInterfaceLandedConstraint::TestConstraint_Implementation(
	const FCL_LandedConstraintContext& ConstraintContext) const
{
	// If the Hit Actor isn't passed we don't fail the flow
	if (ConstraintContext.HitActor == nullptr)
	{
		return false;
	}

	const bool bImplementsInterface = ConstraintContext.HitActor->GetClass()->ImplementsInterface(UCL_SoftLandingInterface::StaticClass());
	
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
