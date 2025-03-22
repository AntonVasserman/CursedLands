// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Attributes/CLAttributeSet.h"

#include "CLGameplayTags.h"
#include "GameplayAbilities/Public/GameplayEffectExtension.h"

UCLAttributeSet::UCLAttributeSet()
{
	// Secondary Attributes
	TagToAttributeMapping.Add(FCLGameplayTags::Get().Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagToAttributeMapping.Add(FCLGameplayTags::Get().Attributes_Secondary_MaxStamina, GetMaxStaminaAttribute);
	
	// Vital Attributes
	TagToAttributeMapping.Add(FCLGameplayTags::Get().Attributes_Vital_Health, GetHealthAttribute);
	TagToAttributeMapping.Add(FCLGameplayTags::Get().Attributes_Vital_Stamina, GetStaminaAttribute);
}

FGameplayAttribute UCLAttributeSet::GetAttributeByGameplayTag(const FGameplayTag& AttributeGameplayTag) const
{
	checkf(AttributeGameplayTag.MatchesTag(FCLGameplayTags::Get().Attributes), TEXT("Trying to get Attribute by Gameplay Tag with a non-attribute Gameplay Tag: %s"), *AttributeGameplayTag.ToString());
	return TagToAttributeMapping.FindChecked(AttributeGameplayTag)();
}

//~ UAttributeSet Begin

void UCLAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
	}
}

void UCLAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
}

//~ UAttributeSet End
