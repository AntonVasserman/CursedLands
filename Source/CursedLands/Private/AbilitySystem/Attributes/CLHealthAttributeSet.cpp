// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Attributes/CLHealthAttributeSet.h"

#include "CLGameplayTags.h"
#include "GameplayEffectExtension.h"

UCLHealthAttributeSet::UCLHealthAttributeSet()
{
	TagToAttributeMapping.Add(FCLGameplayTags::Get().Attributes_General_Health, GetHealthAttribute);
	TagToAttributeMapping.Add(FCLGameplayTags::Get().Attributes_General_MaxHealth, GetMaxHealthAttribute);
}

void UCLHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
}

void UCLHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
}
