// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CL_AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class CURSEDLANDS_API UCL_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UCL_AttributeSet();
	
	TMap<FGameplayTag, TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr> TagToAttributeMapping;
	
	UFUNCTION(BlueprintPure, Category = "Gameplay Ability System|Attributes")
	FGameplayAttribute GetAttributeByGameplayTag(const FGameplayTag& AttributeGameplayTag) const;

	//~ UAttributeSet Begin
	//~ UAttributeSet End
};
