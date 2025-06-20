// Copyright Anton Vasserman, All Rights Reserved.


#include "Systems/Falling/Tasks/CL_ApplyGameplayEffectLandedTask.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/CL_GameplayAbilitySystemStatics.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Systems/Falling/Components/CL_CharacterFallingComponent.h"

//~ UCLLandedTaskBase Begin

float UCL_ApplyGameplayEffectLandedTask::GetLevelToApply(const FCL_LandedTaskContext& TaskContext) const
{
	switch (GameplayEffectLevelType)
	{
	case ECL_GameplayEffectLevelType::Absolute:
		return GameplayEffectLevel;
	case ECL_GameplayEffectLevelType::RelativeToFallHeight:
		{
			const float NormalizedFallHeight = UKismetMathLibrary::NormalizeToRange(TaskContext.FallHeight, MinFallHeight, MaxFallHeight);
			return FMath::Clamp(NormalizedFallHeight, MinGameplayEffectLevel, MaxGameplayEffectLevel);
		}
	default:
		// Should never reach here
		checkNoEntry();
		return 0.f;
	}
}

void UCL_ApplyGameplayEffectLandedTask::ExecuteTaskInternal_Implementation(const FCL_LandedTaskContext& TaskContext) const
{
	checkf(TaskContext.Character, TEXT("TaskContext.Character uninitialized in ApplyGameplayEffectLandedTask"));

	UAbilitySystemComponent* AbilitySystemComponent = UCL_GameplayAbilitySystemStatics::GetAbilitySystemComponent(TaskContext.Character);

	// If we can't find the Component we won't throw, we will just not apply the effect
	if (AbilitySystemComponent == nullptr)
	{
		UE_LOG(LogCharacterFalling, Warning, TEXT("Couldn't find AbilitySystemComponent on for Character: %s, in ApplyGameplayEffectLandedTask"), *TaskContext.Character->GetFullName());
		return;
	}
	
	// On the contrary, if the user creates a task, he must assign a gameplay effect to be used with it
	checkf(GameplayEffectClass, TEXT("GameplayEffectClass uninitialized in ApplyGameplayEffectLandedTask for Character: %s"), *TaskContext.Character->GetFullName());

	// TODO (CL-167): Extract to a GAS Operation :)
	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(TaskContext.FallingComponent);
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, GetLevelToApply(TaskContext), ContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), AbilitySystemComponent);
}

//~ UCLLandedTaskBase End
