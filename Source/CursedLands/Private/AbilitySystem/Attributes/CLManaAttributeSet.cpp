// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Attributes/CLManaAttributeSet.h"

#include "CLGameplayTags.h"
#include "GameplayEffectExtension.h"

UCLManaAttributeSet::UCLManaAttributeSet()
{
	TagToAttributeMapping.Add(CLGameplayTags::Attributes_General_Mana, GetManaAttribute);
	TagToAttributeMapping.Add(CLGameplayTags::Attributes_General_MaxMana, GetMaxManaAttribute);
}
