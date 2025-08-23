// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Attributes/CL_StaminaAttributeSet.h"

#include "CL_GameplayTags.h"

UCL_StaminaAttributeSet::UCL_StaminaAttributeSet()
{
	TagToAttributeMapping.Add(CLGameplayTags::Attributes_General_Stamina, GetStaminaAttribute);
	TagToAttributeMapping.Add(CLGameplayTags::Attributes_General_MaxStamina, GetMaxStaminaAttribute);
	ResourceInfiniteGameplayTag = CLGameplayTags::Resource_Stamina_Infinite;
}
