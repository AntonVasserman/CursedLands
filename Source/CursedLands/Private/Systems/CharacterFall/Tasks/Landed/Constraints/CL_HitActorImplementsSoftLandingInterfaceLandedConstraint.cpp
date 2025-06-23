// Copyright Anton Vasserman, All Rights Reserved.


#include "Systems/CharacterFall/Tasks/Landed/Constraints/CL_HitActorImplementsSoftLandingInterfaceLandedConstraint.h"

#include "Systems/CharacterFall/CL_SoftLandingInterface.h"

//~ UAV_LandedConstraintBase Begin

bool UCL_HitActorImplementsSoftLandingInterfaceLandedConstraint::TestConstraint_Implementation(const FAV_LandedConstraintContext& ConstraintContext) const
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

//~ UAV_LandedConstraintBase End
