// Copyright Anton Vasserman, All Rights Reserved.

#include "Systems/Falling/Tasks/CL_LandedTaskBase.h"

#include "Systems/Falling/Tasks/Landed/Constraints/CL_LandedConstraintBase.h"

void UCL_LandedTaskBase::ExecuteTask(const FCL_LandedTaskContext& TaskContext) const
{
	if (bHasConstraint)
	{
		for (const TObjectPtr<UCL_LandedConstraintBase> Constraint : Constraints)
		{
			FCL_LandedConstraintContext ConstraintContext;
			ConstraintContext.Character = TaskContext.Character;
			ConstraintContext.HitActor = TaskContext.HitActor;
			
			if (Constraint->TestConstraint(ConstraintContext) == false)
			{
				return;
			}
		}
	}

	return ExecuteTaskInternal(TaskContext);
}
