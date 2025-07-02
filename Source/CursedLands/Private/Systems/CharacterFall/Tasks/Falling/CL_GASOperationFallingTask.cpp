// Copyright Anton Vasserman, All Rights Reserved.


#include "Systems/CharacterFall/Tasks/Falling/CL_GASOperationFallingTask.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AV_CharacterFallLogChannels.h"
#include "AbilitySystem/Operations/CL_GASOperationBase.h"
#include "GameFramework/Character.h"

//~ UAV_FallingTaskBase Begin

void UCL_GASOperationFallingTask::ExecuteTask_Implementation(const FAV_FallingTaskContext& TaskContext) const
{
	checkf(TaskContext.Character, TEXT("%s::%hs: TaskContext.Character uninitialized"), *GetClass()->GetName(), __FUNCTION__);

	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TaskContext.Character);

	// If we can't find the Component we won't throw, we will just not apply the effect
	if (AbilitySystemComponent == nullptr)
	{
		UE_LOG(LogAV_CharacterFall, Warning, TEXT("%s::%hs: Couldn't find AbilitySystemComponent on for Character: %s"), *GetClass()->GetName(), __FUNCTION__, *TaskContext.Character->GetFullName());
		return;
	}
	
	// On the contrary, if the user creates a task, he must assign a gameplay effect to be used with it
	checkf(GASOperation, TEXT("%s::%hs: Operation uninitialized for Character: %s"), *GetClass()->GetName(), __FUNCTION__, *TaskContext.Character->GetFullName());

	GASOperation->Execute(AbilitySystemComponent);
}

//~ UAV_FallingTaskBase End
