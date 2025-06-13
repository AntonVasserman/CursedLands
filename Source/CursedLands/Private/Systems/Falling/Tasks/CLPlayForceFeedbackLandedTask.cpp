// Copyright Anton Vasserman, All Rights Reserved.

#include "Systems/Falling/Tasks/CLPlayForceFeedbackLandedTask.h"

#include "Characters/Components/CLCharacterFallingComponent.h"

void UCLPlayForceFeedbackLandedTask::ExecuteTask(const FCLLandedTaskContext& TaskContext) const
{
	UCLLandedTaskBase::ExecuteTask(TaskContext);

	// We allow the PlayerController to be null here, so in case the Task is used on a non PlayerController
	// we won't crush, but just don't play the ForceFeedback.
	if (TaskContext.PlayerController == nullptr)
	{
		// TODO (CL-153): Add maybe a Character Context here so we can log it
		UE_LOG(LogCharacterFalling, Warning, TEXT("PlayerController is nullptr in PlayForceFeedbackLandedTask"));
		return;
	}

	// On the contrary, we do not want to create a task with an empty Force Feedback.
	// TODO (CL-153): Add maybe a Character Context here so we can log it
	checkf(ForceFeedbackEffect, TEXT("ForceFeedbackEffect uninitialized in PlayForceFeedbackLandedTask for PlayerController: %s"), *TaskContext.PlayerController->GetFullName());
	
	// No need to check the feedback here for nullptr, as it is allowed to be nullptr in ClientPlayForceFeedback.
	TaskContext.PlayerController->ClientPlayForceFeedback(ForceFeedbackEffect);
}
