// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Attributes/CLStaminaAttributeSet.h"

#include "CLGameplayTags.h"
#include "GameplayEffectExtension.h"

UCLStaminaAttributeSet::UCLStaminaAttributeSet()
{
	TagToAttributeMapping.Add(FCLGameplayTags::Get().Attributes_General_Stamina, GetStaminaAttribute);
	TagToAttributeMapping.Add(FCLGameplayTags::Get().Attributes_General_MaxStamina, GetMaxStaminaAttribute);
}

void UCLStaminaAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
	}
}

void UCLStaminaAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
}
