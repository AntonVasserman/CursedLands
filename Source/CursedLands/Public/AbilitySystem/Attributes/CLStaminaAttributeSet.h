// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLResourceAttributeSet.h"
#include "CLStaminaAttributeSet.generated.h"

UCLASS()
class CURSEDLANDS_API UCLStaminaAttributeSet : public UCLResourceAttributeSet
{
	GENERATED_BODY()

public:
	UCLStaminaAttributeSet();
	
	ATTRIBUTE_ACCESSORS(UCLStaminaAttributeSet, Stamina);
	ATTRIBUTE_ACCESSORS(UCLStaminaAttributeSet, MaxStamina);

private:
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Ability System|Attributes", Meta = (AllowPrivateAccess))
	FGameplayAttributeData Stamina;
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Ability System|Attributes", Meta = (AllowPrivateAccess))
	FGameplayAttributeData MaxStamina;
	
	//~ UCLResourceAttributeSet Begin
public:
	virtual float GetValue() const override { return GetStamina(); }
	virtual void SetValue(float NewValue) override { SetStamina(NewValue); }
	virtual FGameplayAttribute GetValueAttribute() const override { return GetStaminaAttribute(); }
	virtual float GetMaxValue() const override { return GetMaxStamina(); }
	virtual void SetMaxValue(float NewValue) override { SetMaxStamina(NewValue); }
	virtual FGameplayAttribute GetMaxValueAttribute() const override { return GetMaxStaminaAttribute(); }
	//~ UCLResourceAttributeSet End
};
