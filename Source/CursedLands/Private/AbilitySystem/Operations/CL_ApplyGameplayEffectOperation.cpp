// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Operations/CL_ApplyGameplayEffectOperation.h"

#include "AbilitySystemComponent.h"

void UCL_ApplyGameplayEffectOperation::Execute_Implementation(UAbilitySystemComponent* InAbilitySystemComponent) const
{
	checkf(InAbilitySystemComponent, TEXT("%s::%hs: AbilitySystemComponent uninitialized"), *GetClass()->GetName(), __FUNCTION__);
	checkf(GameplayEffectClass, TEXT("%s::%hs: GameplayEffectClass uninitialized"), *GetClass()->GetName(), __FUNCTION__);

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
