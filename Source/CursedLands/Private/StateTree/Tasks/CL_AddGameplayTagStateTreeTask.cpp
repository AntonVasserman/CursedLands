// Copyright Anton Vasserman, All Rights Reserved.


#include "StateTree/Tasks/CL_AddGameplayTagStateTreeTask.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CL_LogChannels.h"
#include "StateTreeExecutionContext.h"

FCL_AddGameplayTagStateTreeTask::FCL_AddGameplayTagStateTreeTask()
{
	bShouldCallTick = false;
}

EStateTreeRunStatus FCL_AddGameplayTagStateTreeTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InstanceData.Actor);

	// If we can't find the Component we won't throw, we will just not add the Tag
	if (AbilitySystemComponent == nullptr)
	{
		UE_LOG(LogCL, Warning, TEXT("%s::%hs: Couldn't find AbilitySystemComponent on for Actor: %s"), *StaticStruct()->GetName(), __FUNCTION__, *InstanceData.Actor->GetFullName());
		return EStateTreeRunStatus::Failed;
	}

	AbilitySystemComponent->SetLooseGameplayTagCount(InstanceData.GameplayTag, 1);
	
	return EStateTreeRunStatus::Succeeded;
}

void FCL_AddGameplayTagStateTreeTask::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InstanceData.Actor);

	// If we can't find the Component we won't throw, we will just not add the Tag
	if (AbilitySystemComponent == nullptr)
	{
		UE_LOG(LogCL, Warning, TEXT("%s::%hs: Couldn't find AbilitySystemComponent on for Actor: %s"), *StaticStruct()->GetName(), __FUNCTION__, *InstanceData.Actor->GetFullName());
	}

	if (bRemoveOnExit)
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(InstanceData.GameplayTag);
	}
}
