// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/CLAbilitySystemComponent.h"

bool UCLAbilitySystemComponent::AddUniqueGameplayTag(const FGameplayTag& GameplayTag)
{
	if (HasMatchingGameplayTag(GameplayTag))
	{
		return false;
	}
	
	AddLooseGameplayTag(GameplayTag);
	return true;
}

bool UCLAbilitySystemComponent::RemoveGameplayTag(const FGameplayTag& GameplayTag)
{
	if (!HasMatchingGameplayTag(GameplayTag))
	{
		return false;
	}
	
	RemoveLooseGameplayTag(GameplayTag);
	return true;
}

//~ UAbilitySystemComponent Begin
//~ UAbilitySystemComponent End
