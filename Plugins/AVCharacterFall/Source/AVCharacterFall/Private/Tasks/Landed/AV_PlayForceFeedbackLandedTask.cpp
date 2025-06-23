// Copyright Anton Vasserman, All Rights Reserved.

#include "Tasks/Landed/AV_PlayForceFeedbackLandedTask.h"

#include "Components/AV_CharacterFallComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

//~ UAV_LandedTaskBase Begin

void UAV_PlayForceFeedbackLandedTask::ExecuteTaskInternal_Implementation(const FAV_LandedTaskContext& TaskContext) const
{
	checkf(TaskContext.Character, TEXT("%s::%hs: TaskContext.Character uninitialized"), *GetClass()->GetName(), __FUNCTION__);

	APlayerController* PlayerController = Cast<APlayerController>(TaskContext.Character->GetController());
	
	// We allow the PlayerController to be null here, so in case the Task is used on a non PlayerController
	// we won't crush, but just don't play the ForceFeedback.
	if (PlayerController == nullptr)
	{
		UE_LOG(LogAV_CharacterFall, Warning, TEXT("%s::%hs: PlayerController is nullptr in PlayForceFeedbackLandedTask for Character: %s"), *GetClass()->GetName(), __FUNCTION__, *TaskContext.Character->GetFullName());
		return;
	}

	// On the contrary, we do not want to create a task with an empty Force Feedback.
	checkf(ForceFeedbackEffect, TEXT("%s::%hs: ForceFeedbackEffect uninitialized for Character: %s"), *GetClass()->GetName(), __FUNCTION__, *TaskContext.Character->GetFullName());
	
	// No need to check the feedback here for nullptr, as it is allowed to be nullptr in ClientPlayForceFeedback.
	PlayerController->ClientPlayForceFeedback(ForceFeedbackEffect);
}

//~ UAV_LandedTaskBase End
