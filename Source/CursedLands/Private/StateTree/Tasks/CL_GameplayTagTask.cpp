// Copyright Anton Vasserman, All Rights Reserved.


#include "StateTree/Tasks/CL_GameplayTagTask.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CL_LogChannels.h"
#include "StateTreeExecutionContext.h"
#include "Controllers/CL_PlayerController.h"

FCL_GameplayTagTask::FCL_GameplayTagTask()
{
	bShouldCallTick = false;

#if WITH_EDITORONLY_DATA
	bConsideredForCompletion = false;
	bCanEditConsideredForCompletion = false;
#endif
}

EStateTreeRunStatus FCL_GameplayTagTask::EnterState(
	FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	AActor* Actor = InstanceData.Controller->GetPawn();
	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);

	// If we can't find the Component we won't throw, we will just not add the Tag
	if (AbilitySystemComponent == nullptr)
	{
		UE_LOG(LogCL, Warning, TEXT("%s::%hs: Couldn't find AbilitySystemComponent on for Actor: %s"), *StaticStruct()->GetName(), __FUNCTION__, *Actor->GetFullName());
		return EStateTreeRunStatus::Failed;
	}

	AbilitySystemComponent->SetLooseGameplayTagCount(InstanceData.GameplayTag, 1);
	
	return EStateTreeRunStatus::Running;
}

void FCL_GameplayTagTask::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	AActor* Actor = InstanceData.Controller->GetPawn();
	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);

	// If we can't find the Component we won't throw, we will just not add the Tag
	if (AbilitySystemComponent == nullptr)
	{
		UE_LOG(LogCL, Warning, TEXT("%s::%hs: Couldn't find AbilitySystemComponent on for Actor: %s"), *StaticStruct()->GetName(), __FUNCTION__, *Actor->GetFullName());
	}
	
	AbilitySystemComponent->RemoveLooseGameplayTag(InstanceData.GameplayTag);
}
