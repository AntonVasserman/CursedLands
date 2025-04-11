// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Attributes/CLManaAttributeSet.h"

#include "CLGameplayTags.h"
#include "GameplayEffectExtension.h"

//~ UCLAttributeSet Begin

UCLManaAttributeSet::UCLManaAttributeSet()
{
	TagToAttributeMapping.Add(CLGameplayTags::Attributes_General_Mana, GetManaAttribute);
	TagToAttributeMapping.Add(CLGameplayTags::Attributes_General_MaxMana, GetMaxManaAttribute);
}

void UCLManaAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
}

void UCLManaAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

//~ UCLAttributeSet End