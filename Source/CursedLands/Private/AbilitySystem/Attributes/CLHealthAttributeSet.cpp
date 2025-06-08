// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Attributes/CLHealthAttributeSet.h"

#include "CLGameplayTags.h"

UCLHealthAttributeSet::UCLHealthAttributeSet()
{
	TagToAttributeMapping.Add(CLGameplayTags::Attributes_General_Health, GetHealthAttribute);
	TagToAttributeMapping.Add(CLGameplayTags::Attributes_General_MaxHealth, GetMaxHealthAttribute);
}
