// Copyright Anton Vasserman, All Rights Reserved.

#include "Components/Models/AV_FallRangeTasks.h"

#include "Tasks/Falling/AV_FallingTaskBase.h"
#include "Tasks/Landed/AV_LandedTaskBase.h"

EAV_FallRangeState FAV_FallRangeTasks::TestRange(const FAV_FallRangeContext& ConditionContext)
{
	if (ConditionContext.HitActor != nullptr)
	{
		if (State == EAV_FallRangeState::Inside)
		{
			ExecuteFallingExitTasks(ConditionContext);
			ExecuteLandedTasks(ConditionContext);
		}
		
		State = EAV_FallRangeState::Outside;
		return State;
	}

	const bool bInRange = FallRange.TestRange(ConditionContext.FallHeight);
	switch (State)
	{
	case EAV_FallRangeState::Outside:
		if (bInRange)
		{
			ExecuteFallingEnterTasks(ConditionContext);
			State = EAV_FallRangeState::Inside;
		}
		break;
	case EAV_FallRangeState::Inside:
		if (!bInRange)
		{
			ExecuteFallingExitTasks(ConditionContext);
			State = EAV_FallRangeState::Outside;
		}
		break;
	default:
		checkNoEntry();
	}

	return State;
}

void FAV_FallRangeTasks::ExecuteLandedTasks(const FAV_FallRangeContext& ConditionContext)
{
	FAV_LandedTaskContext TaskContext;
	TaskContext.Character = ConditionContext.Character;
	TaskContext.HitActor = ConditionContext.HitActor;
	TaskContext.FallHeight = ConditionContext.FallHeight;
	TaskContext.FallingComponent = ConditionContext.CharacterFallComponent;
			
	for (const UAV_LandedTaskBase* Task : LandedTasks)
	{
		Task->ExecuteTask(TaskContext);
	}
}

void FAV_FallRangeTasks::ExecuteFallingEnterTasks(const FAV_FallRangeContext& ConditionContext)
{
	FAV_FallingTaskContext TaskContext;
	TaskContext.Character = ConditionContext.Character;
	TaskContext.FallingComponent = ConditionContext.CharacterFallComponent;
	for (const UAV_FallingTaskBase* Task : FallingEnterTasks)
	{
		Task->ExecuteTask(TaskContext);
	}
}

void FAV_FallRangeTasks::ExecuteFallingExitTasks(const FAV_FallRangeContext& ConditionContext)
{
	FAV_FallingTaskContext TaskContext;
	TaskContext.Character = ConditionContext.Character;
	TaskContext.FallingComponent = ConditionContext.CharacterFallComponent;
	for (const UAV_FallingTaskBase* Task : FallingExitTasks)
	{
		Task->ExecuteTask(TaskContext);
	}
}
