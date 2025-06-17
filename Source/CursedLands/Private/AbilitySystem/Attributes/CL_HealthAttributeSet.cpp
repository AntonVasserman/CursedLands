// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Attributes/CL_HealthAttributeSet.h"

#include "CLGameplayTags.h"

UCL_HealthAttributeSet::UCL_HealthAttributeSet()
{
	TagToAttributeMapping.Add(CLGameplayTags::Attributes_General_Health, GetHealthAttribute);
	TagToAttributeMapping.Add(CLGameplayTags::Attributes_General_MaxHealth, GetMaxHealthAttribute);
}
