// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_ResourceAttributeSet.h"
#include "CL_StaminaAttributeSet.generated.h"

UCLASS()
class CURSEDLANDS_API UCL_StaminaAttributeSet : public UCL_ResourceAttributeSet
{
	GENERATED_BODY()

public:
	UCL_StaminaAttributeSet();
	
	ATTRIBUTE_ACCESSORS(UCL_StaminaAttributeSet, Stamina);
	ATTRIBUTE_ACCESSORS(UCL_StaminaAttributeSet, MaxStamina);

private:
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Ability System|Attributes", Meta = (AllowPrivateAccess))
	FGameplayAttributeData Stamina;
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Ability System|Attributes", Meta = (AllowPrivateAccess))
	FGameplayAttributeData MaxStamina;
	
	//~ UCL_ResourceComponent Begin
public:
	virtual float GetValue() const override { return GetStamina(); }
	virtual void SetValue(float NewValue) override { SetStamina(NewValue); }
	virtual FGameplayAttribute GetValueAttribute() const override { return GetStaminaAttribute(); }
	virtual float GetMaxValue() const override { return GetMaxStamina(); }
	virtual void SetMaxValue(float NewValue) override { SetMaxStamina(NewValue); }
	virtual FGameplayAttribute GetMaxValueAttribute() const override { return GetMaxStaminaAttribute(); }
	//~ UCL_ResourceComponent End
};
