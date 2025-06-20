// Copyright Anton Vasserman, All Rights Reserved.


#include "Systems/Falling/Tasks/CL_GASOperationFallingTask.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/CL_GameplayAbilitySystemStatics.h"
#include "AbilitySystem/Operations/CL_GameplayTagOperation.h"
#include "GameFramework/Character.h"
#include "Systems/Falling/Components/CL_CharacterFallingComponent.h"

void UCL_GASOperationFallingTask::ExecuteTask_Implementation(const FCL_FallingTaskContext& TaskContext) const
{
	checkf(TaskContext.Character, TEXT("TaskContext.Character uninitialized in GameplayTagOperationFallingTask"));

	UAbilitySystemComponent* AbilitySystemComponent = UCL_GameplayAbilitySystemStatics::GetAbilitySystemComponent(TaskContext.Character);

	// If we can't find the Component we won't throw, we will just not apply the effect
	if (AbilitySystemComponent == nullptr)
	{
		UE_LOG(LogCharacterFalling, Warning, TEXT("Couldn't find AbilitySystemComponent on for Character: %s, in GameplayTagOperationFallingTask"), *TaskContext.Character->GetFullName());
		return;
	}
	
	// On the contrary, if the user creates a task, he must assign a gameplay effect to be used with it
	checkf(GASOperation, TEXT("Operation uninitialized in GameplayTagOperationFallingTask for Character: %s"), *TaskContext.Character->GetFullName());

	GASOperation->Execute(AbilitySystemComponent);
}
