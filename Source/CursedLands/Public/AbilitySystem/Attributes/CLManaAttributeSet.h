// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLAttributeSet.h"
#include "CLManaAttributeSet.generated.h"

UCLASS()
class CURSEDLANDS_API UCLManaAttributeSet : public UCLAttributeSet
{
	GENERATED_BODY()

public:
	UCLManaAttributeSet();
	
	ATTRIBUTE_ACCESSORS(UCLManaAttributeSet, Mana);
	ATTRIBUTE_ACCESSORS(UCLManaAttributeSet, MaxMana);
	
private:
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Ability System|Attributes", Meta = (AllowPrivateAccess))
	FGameplayAttributeData Mana;
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Ability System|Attributes", Meta = (AllowPrivateAccess))
	FGameplayAttributeData MaxMana;

	//~ UCLAttributeSet Begin
public:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//~ UCLAttributeSet End
};
