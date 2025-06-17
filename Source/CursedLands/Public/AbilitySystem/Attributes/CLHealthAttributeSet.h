// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLResourceAttributeSet.h"
#include "CLHealthAttributeSet.generated.h"

UCLASS()
class CURSEDLANDS_API UCLHealthAttributeSet : public UCLResourceAttributeSet
{
	GENERATED_BODY()

public:
	UCLHealthAttributeSet();

	ATTRIBUTE_ACCESSORS(UCLHealthAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UCLHealthAttributeSet, MaxHealth);
	
private:
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Ability System|Attributes", Meta = (AllowPrivateAccess))
	FGameplayAttributeData Health;
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Ability System|Attributes", Meta = (AllowPrivateAccess))
	FGameplayAttributeData MaxHealth;

	//~ UCLResourceAttributeSet Begin
public:
	virtual float GetValue() const override { return GetHealth(); }
	virtual void SetValue(float NewValue) override { SetHealth(NewValue); }
	virtual FGameplayAttribute GetValueAttribute() const override { return GetHealthAttribute(); }
	virtual float GetMaxValue() const override { return GetMaxHealth(); }
	virtual void SetMaxValue(float NewValue) override { SetMaxHealth(NewValue); }
	virtual FGameplayAttribute GetMaxValueAttribute() const override { return GetMaxHealthAttribute(); }
	//~ UCLResourceAttributeSet End
};
