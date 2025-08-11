// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Operations/CL_ApplyGameplayEffectOperation.h"

#include "AbilitySystemComponent.h"

void UCL_ApplyGameplayEffectOperation::Execute_Implementation(UAbilitySystemComponent* InAbilitySystemComponent) const
{
	checkf(InAbilitySystemComponent, TEXT("%s: AbilitySystemComponent uninitialized"), __FUNCTIONW__);
	checkf(GameplayEffectClass, TEXT("%s: GameplayEffectClass uninitialized"), *GetClass()->GetName(), __FUNCTIONW__);

	const FGameplayEffectContextHandle ContextHandle = InAbilitySystemComponent->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = InAbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, GameplayEffectLevel, ContextHandle);
	InAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), InAbilitySystemComponent);
}

FString UCL_ApplyGameplayEffectOperation::GetOperationName_Implementation() const
{
	FString GameplayEffectClassName = TEXT("None");
	if (GameplayEffectClass)
	{
		GameplayEffectClassName = GameplayEffectClass->GetName();
	}
	
	return FString::Printf(TEXT("Apply Gameplay Effect: %s at Level: %f"), *GameplayEffectClassName, GameplayEffectLevel);
}
