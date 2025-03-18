// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CLAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class CURSEDLANDS_API UCLAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UCLAttributeSet();

	TMap<FGameplayTag, TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr> TagToAttributeMapping;
	
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Ability System | Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCLAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Ability System | Secondary Attributes")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UCLAttributeSet, MaxStamina);

	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Ability System | Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCLAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Ability System | Vital Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UCLAttributeSet, Stamina);

	//~ UAttributeSet Begin

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	//~ UAttributeSet End
};
