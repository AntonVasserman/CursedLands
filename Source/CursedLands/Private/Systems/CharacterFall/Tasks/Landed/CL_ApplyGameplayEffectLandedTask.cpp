// Copyright Anton Vasserman, All Rights Reserved.


#include "Systems/CharacterFall/Tasks/Landed/CL_ApplyGameplayEffectLandedTask.h"

#include "AbilitySystemComponent.h"
#include "AV_CharacterFallLogChannels.h"
#include "AbilitySystem/CL_GameplayAbilitySystemStatics.h"
#include "Components/AV_CharacterFallComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

float UCL_ApplyGameplayEffectLandedTask::GetLevelToApply(const FAV_LandedTaskContext& TaskContext) const
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

//~ UAV_LandedTaskBase Begin

void UCL_ApplyGameplayEffectLandedTask::ExecuteTaskInternal_Implementation(
	const FAV_LandedTaskContext& TaskContext) const
{
	checkf(TaskContext.Character, TEXT("%s::%hs: TaskContext.Character uninitialized"), *GetClass()->GetName(), __FUNCTION__);

	UAbilitySystemComponent* AbilitySystemComponent = UCL_GameplayAbilitySystemStatics::GetAbilitySystemComponent(TaskContext.Character);

	// If we can't find the Component we won't throw, we will just not apply the effect
	if (AbilitySystemComponent == nullptr)
	{
		UE_LOG(LogAV_CharacterFall, Warning, TEXT("%s::%hs: Couldn't find AbilitySystemComponent on for Character: %s"), *GetClass()->GetName(), __FUNCTION__, *TaskContext.Character->GetFullName());
		return;
	}
	
	// On the contrary, if the user creates a task, he must assign a gameplay effect to be used with it
	checkf(GameplayEffectClass, TEXT("%s::%hs: GameplayEffectClass uninitialized for Character: %s"), *GetClass()->GetName(), __FUNCTION__, *TaskContext.Character->GetFullName());

	// TODO (CL-167): Extract to a GAS Operation :)
	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(TaskContext.FallingComponent);
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, GetLevelToApply(TaskContext), ContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), AbilitySystemComponent);
}

//~ UAV_LandedTaskBase End
