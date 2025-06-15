// Copyright Anton Vasserman, All Rights Reserved.

#include "Systems/Falling/Conditions/CLHitActorImplementsInterfaceLandedCondition.h"

#include "GameFramework/Character.h"

bool UCLHitActorImplementsInterfaceLandedCondition::TestCondition(
	const FCLLandedConditionContext& ConditionContext) const
{
	// If the Hit Actor isn't passed we don't fail the flow
	if (ConditionContext.HitActor == nullptr)
	{
		return false;
	}

	// On the contrary, the Interface to be checked must be set if this condition is used
	checkf(InterfaceClass, TEXT("InterfaceClass uninitialized in HitActorImplementsInterfaceLandedCondition for Character: %s"), *ConditionContext.Character->GetFullName());

	return ConditionContext.HitActor->GetClass()->ImplementsInterface(InterfaceClass);
}
