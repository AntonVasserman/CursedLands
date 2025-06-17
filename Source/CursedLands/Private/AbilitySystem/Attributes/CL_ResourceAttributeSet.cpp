// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Attributes/CL_ResourceAttributeSet.h"

#include "GameplayEffectExtension.h"

//~ UCL_AttributeSet Begin

void UCL_ResourceAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetValueAttribute())
	{
		SetValue(FMath::Clamp(GetValue(), 0.f, GetMaxValue()));
	}
}

void UCL_ResourceAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetValueAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxValue());
	}
}

//~ UCL_AttributeSet End