// Copyright Anton Vasserman, All Rights Reserved.


#include "Tasks/Landed/AV_LandedTaskBase.h"

#include "Tasks/Landed/Constraints/AV_LandedConstraintBase.h"

void UAV_LandedTaskBase::ExecuteTask(const FAV_LandedTaskContext& TaskContext) const
{
	if (bHasConstraint)
	{
		for (const UAV_LandedConstraintBase* Constraint : Constraints)
		{
			FAV_LandedConstraintContext ConstraintContext;
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
