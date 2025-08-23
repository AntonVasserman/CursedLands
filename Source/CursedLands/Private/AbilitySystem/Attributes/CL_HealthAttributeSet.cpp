// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Attributes/CL_HealthAttributeSet.h"

#include "CL_GameplayTags.h"

UCL_HealthAttributeSet::UCL_HealthAttributeSet()
{
	TagToAttributeMapping.Add(CLGameplayTags::Attributes_General_Health, GetHealthAttribute);
	TagToAttributeMapping.Add(CLGameplayTags::Attributes_General_MaxHealth, GetMaxHealthAttribute);
	ResourceInfiniteGameplayTag = CLGameplayTags::Resource_Health_Infinite;
}
