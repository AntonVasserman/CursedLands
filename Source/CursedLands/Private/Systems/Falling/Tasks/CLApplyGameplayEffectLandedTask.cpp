// Copyright Anton Vasserman, All Rights Reserved.


#include "Systems/Falling/Tasks/CLApplyGameplayEffectLandedTask.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Characters/Components/CLCharacterFallingComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

//~ UCLLandedTaskBase Begin

TOptional<UAbilitySystemComponent*> UCLApplyGameplayEffectLandedTask::GetAbilitySystemComponent(const ACharacter* Character) const
{
	// Try to look for the AbilitySystemComponent by casting to IAbilitySystemInterface
	if (const IAbilitySystemInterface* CharacterAsAbilitySystemInterface = Cast<IAbilitySystemInterface>(Character);
		CharacterAsAbilitySystemInterface != nullptr)
	{
		// UE_LOG(LogCharacterFalling, Warning, TEXT("AbilitySystem is nullptr in ApplyGameplayEffectLandedTask for Character: %s"), *Character->GetFullName());
		// return Result;

		if (UAbilitySystemComponent* AbilitySystemComponent = CharacterAsAbilitySystemInterface->GetAbilitySystemComponent();
			AbilitySystemComponent != nullptr)
		{
			return AbilitySystemComponent;
		}
	}

	// Try to look for the AbilitySystemComponent by looking for it
	if (UAbilitySystemComponent* AbilitySystemComponent = Character->FindComponentByClass<UAbilitySystemComponent>();
		AbilitySystemComponent != nullptr)
	{
		return AbilitySystemComponent;
	}
	
	return TOptional<UAbilitySystemComponent*>();
}

float UCLApplyGameplayEffectLandedTask::GetLevelToApply(const FCLLandedTaskContext& TaskContext) const
{
	switch (GameplayEffectLevelType)
	{
	case ECLGameplayEffectLevelType::Absolute:
		return GameplayEffectLevel;
	case ECLGameplayEffectLevelType::RelativeToFallHeight:
		{
			if (TaskContext.Condition.Condition != ECLFallHeightLandedCondition::InRange)
			{
				return MaxGameplayEffectLevel;
			}
		
			const float NormalizedFallHeight = UKismetMathLibrary::NormalizeToRange(TaskContext.FallHeight, TaskContext.Condition.FallHeightThresholdMin, TaskContext.Condition.FallHeightThresholdMax);
			return FMath::Clamp(NormalizedFallHeight, MinGameplayEffectLevel, MaxGameplayEffectLevel);
		}
	default:
		// Should never reach here
		checkNoEntry();
		return 0.f;
	}
}

void UCLApplyGameplayEffectLandedTask::ExecuteTask(const FCLLandedTaskContext& TaskContext) const
{
	Super::ExecuteTask(TaskContext);
	
	checkf(TaskContext.Character, TEXT("TaskContext.Character uninitialized in ApplyGameplayEffectLandedTask"));

	TOptional<UAbilitySystemComponent*> AbilitySystemComponent = GetAbilitySystemComponent(TaskContext.Character);

	// If we can't find the Component we won't throw, we will just not apply the effect
	if (AbilitySystemComponent.IsSet() == false)
	{
		UE_LOG(LogCharacterFalling, Warning, TEXT("Couldn't find AbilitySystemComponent on for Character: %s, in ApplyGameplayEffectLandedTask"), *TaskContext.Character->GetFullName());
		return;
	}
	
	// On the contrary, if the user creates a task, he must assign a gameplay effect to be used with it
	checkf(GameplayEffectClass, TEXT("GameplayEffectClass uninitialized in ApplyGameplayEffectLandedTask for Character: %s"), *TaskContext.Character->GetFullName());

	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent.GetValue()->MakeEffectContext();
	ContextHandle.AddSourceObject(TaskContext.FallingComponent);
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent.GetValue()->MakeOutgoingSpec(GameplayEffectClass, GetLevelToApply(TaskContext), ContextHandle);
	AbilitySystemComponent.GetValue()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), AbilitySystemComponent.GetValue());
}

//~ UCLLandedTaskBase End
