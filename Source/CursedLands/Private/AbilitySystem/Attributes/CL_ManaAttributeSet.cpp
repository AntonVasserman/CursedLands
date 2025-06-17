// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Attributes/CL_ManaAttributeSet.h"

#include "CLGameplayTags.h"

UCL_ManaAttributeSet::UCL_ManaAttributeSet()
{
	TagToAttributeMapping.Add(CLGameplayTags::Attributes_General_Mana, GetManaAttribute);
	TagToAttributeMapping.Add(CLGameplayTags::Attributes_General_MaxMana, GetMaxManaAttribute);
}

//~ UCL_ResourceAttributeSet Begin
//~ UCL_ResourceAttributeSet End
