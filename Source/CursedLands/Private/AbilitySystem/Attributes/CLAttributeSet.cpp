// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Attributes/CLAttributeSet.h"

#include "CLGameplayTags.h"
#include "GameplayAbilities/Public/GameplayEffectExtension.h"

UCLAttributeSet::UCLAttributeSet()
{
}

FGameplayAttribute UCLAttributeSet::GetAttributeByGameplayTag(const FGameplayTag& AttributeGameplayTag) const
{
	// TODO (CL-58): Return back the usage of FindChecked once CLAttributeProgressBar is refactored
	checkf(AttributeGameplayTag.MatchesTag(CLGameplayTags::Attributes), TEXT("Trying to get Attribute by Gameplay Tag with a non-attribute Gameplay Tag: %s"), *AttributeGameplayTag.ToString());
	FGameplayAttribute (*const*X)() = TagToAttributeMapping.Find(AttributeGameplayTag);

	return X != nullptr ? (*X)() : nullptr;
}

//~ UAttributeSet Begin
//~ UAttributeSet End
