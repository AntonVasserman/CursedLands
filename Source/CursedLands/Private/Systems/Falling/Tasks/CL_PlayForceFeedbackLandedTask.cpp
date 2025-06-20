// Copyright Anton Vasserman, All Rights Reserved.

#include "Systems/Falling/Tasks/CL_PlayForceFeedbackLandedTask.h"

#include "GameFramework/Character.h"
#include "Systems/Falling/Components/CL_CharacterFallingComponent.h"

//~ UCLLandedTaskBase Begin

void UCL_PlayForceFeedbackLandedTask::ExecuteTaskInternal_Implementation(const FCL_LandedTaskContext& TaskContext) const
{
	checkf(TaskContext.Character, TEXT("TaskContext.Character uninitialized in PlayForceFeedbackLandedTask"));

	APlayerController* PlayerController = Cast<APlayerController>(TaskContext.Character->GetController());
	
	// We allow the PlayerController to be null here, so in case the Task is used on a non PlayerController
	// we won't crush, but just don't play the ForceFeedback.
	if (PlayerController == nullptr)
	{
		UE_LOG(LogCharacterFalling, Warning, TEXT("PlayerController is nullptr in PlayForceFeedbackLandedTask for Character: %s"), *TaskContext.Character->GetFullName());
		return;
	}

	// On the contrary, we do not want to create a task with an empty Force Feedback.
	checkf(ForceFeedbackEffect, TEXT("ForceFeedbackEffect uninitialized in PlayForceFeedbackLandedTask for Character: %s"), *TaskContext.Character->GetFullName());
	
	// No need to check the feedback here for nullptr, as it is allowed to be nullptr in ClientPlayForceFeedback.
	PlayerController->ClientPlayForceFeedback(ForceFeedbackEffect);
}

//~ UCLLandedTaskBase End
