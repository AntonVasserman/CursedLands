// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Attributes/CL_StaminaAttributeSet.h"

#include "CLGameplayTags.h"
#include "GameplayEffectExtension.h"

UCL_StaminaAttributeSet::UCL_StaminaAttributeSet()
{
	TagToAttributeMapping.Add(CLGameplayTags::Attributes_General_Stamina, GetStaminaAttribute);
	TagToAttributeMapping.Add(CLGameplayTags::Attributes_General_MaxStamina, GetMaxStaminaAttribute);
}
