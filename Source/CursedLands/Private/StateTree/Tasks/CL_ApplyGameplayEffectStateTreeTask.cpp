// Copyright Anton Vasserman, All Rights Reserved.


#include "StateTree/Tasks/CL_ApplyGameplayEffectStateTreeTask.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "StateTreeExecutionContext.h"
#include "GameFramework/Character.h"

FCL_ApplyGameplayEffectStateTreeTask::FCL_ApplyGameplayEffectStateTreeTask()
{
	bShouldCallTick = false;
}

EStateTreeRunStatus FCL_ApplyGameplayEffectStateTreeTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	
	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InstanceData.Character);
	
	if (AbilitySystemComponent == nullptr)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	if (InstanceData.GameplayEffectClass == nullptr)
	{
		return EStateTreeRunStatus::Failed;
	}

	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(InstanceData.GameplayEffectClass, InstanceData.GameplayEffectLevel, ContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), AbilitySystemComponent);

	return EStateTreeRunStatus::Succeeded;
}

#if WITH_EDITOR
FText FCL_ApplyGameplayEffectStateTreeTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	const FInstanceDataType* InstanceData = InstanceDataView.GetPtr<FInstanceDataType>();
	
	if (!InstanceData)
	{
		return FText::FromString(TEXT("Apply Gameplay Effect"));
	}

	FString GameplayEffectClassName = TEXT("None");
	if (InstanceData->GameplayEffectClass)
	{
		GameplayEffectClassName = InstanceData->GameplayEffectClass->GetName();
	}

	return FText::FromString(FString::Printf(TEXT("Apply Gameplay Effect: %s at Level: %f"), *GameplayEffectClassName, InstanceData->GameplayEffectLevel));
}
#endif