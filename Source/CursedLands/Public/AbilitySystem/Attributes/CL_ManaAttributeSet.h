// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_ResourceAttributeSet.h"
#include "CL_ManaAttributeSet.generated.h"

UCLASS()
class CURSEDLANDS_API UCL_ManaAttributeSet : public UCL_ResourceAttributeSet
{
	GENERATED_BODY()

public:
	UCL_ManaAttributeSet();
	
	ATTRIBUTE_ACCESSORS(UCL_ManaAttributeSet, Mana);
	ATTRIBUTE_ACCESSORS(UCL_ManaAttributeSet, MaxMana);
	
private:
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Ability System|Attributes", Meta = (AllowPrivateAccess))
	FGameplayAttributeData Mana;
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Ability System|Attributes", Meta = (AllowPrivateAccess))
	FGameplayAttributeData MaxMana;

	//~ UCL_ResourceAttributeSet Begin
public:
	virtual float GetValue() const override { return GetMana(); }
	virtual void SetValue(float NewValue) override { SetMana(NewValue); }
	virtual FGameplayAttribute GetValueAttribute() const override { return GetManaAttribute(); }
	virtual float GetMaxValue() const override { return GetMaxMana(); }
	virtual void SetMaxValue(float NewValue) override { SetMaxMana(NewValue); }
	virtual FGameplayAttribute GetMaxValueAttribute() const override { return GetMaxManaAttribute(); }
	//~ UCL_ResourceAttributeSet End
};
