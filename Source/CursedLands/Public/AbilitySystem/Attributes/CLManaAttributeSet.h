// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLResourceAttributeSet.h"
#include "CLManaAttributeSet.generated.h"

UCLASS()
class CURSEDLANDS_API UCLManaAttributeSet : public UCLResourceAttributeSet
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

	//~ UCLResourceAttributeSet Begin
public:
	virtual float GetValue() const override { return GetMana(); }
	virtual void SetValue(float NewValue) override { SetMana(NewValue); }
	virtual FGameplayAttribute GetValueAttribute() const override { return GetManaAttribute(); }
	virtual float GetMaxValue() const override { return GetMaxMana(); }
	virtual void SetMaxValue(float NewValue) override { SetMaxMana(NewValue); }
	virtual FGameplayAttribute GetMaxValueAttribute() const override { return GetMaxManaAttribute(); }
	//~ UCLResourceAttributeSet End
};
