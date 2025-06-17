// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Attributes/CLResourceAttributeSet.h"

#include "GameplayEffectExtension.h"

//~ UCLAttributeSet Begin
void UCLResourceAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetValueAttribute())
	{
		SetValue(FMath::Clamp(GetValue(), 0.f, GetMaxValue()));
	}
}

void UCLResourceAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetValueAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxValue());
	}
}
//~ UCLAttributeSet End