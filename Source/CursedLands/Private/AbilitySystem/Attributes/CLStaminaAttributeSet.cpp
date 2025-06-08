// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Attributes/CLStaminaAttributeSet.h"

#include "CLGameplayTags.h"
#include "GameplayEffectExtension.h"

UCLStaminaAttributeSet::UCLStaminaAttributeSet()
{
	TagToAttributeMapping.Add(CLGameplayTags::Attributes_General_Stamina, GetStaminaAttribute);
	TagToAttributeMapping.Add(CLGameplayTags::Attributes_General_MaxStamina, GetMaxStaminaAttribute);
}
