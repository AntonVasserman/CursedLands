// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_ResourceAttributeSet.h"
#include "CL_HealthAttributeSet.generated.h"

UCLASS()
class CURSEDLANDS_API UCL_HealthAttributeSet : public UCL_ResourceAttributeSet
{
	GENERATED_BODY()

public:
	UCL_HealthAttributeSet();

	ATTRIBUTE_ACCESSORS(UCL_HealthAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UCL_HealthAttributeSet, MaxHealth);
	
private:
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Ability System|Attributes", Meta = (AllowPrivateAccess))
	FGameplayAttributeData Health;
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Ability System|Attributes", Meta = (AllowPrivateAccess))
	FGameplayAttributeData MaxHealth;

	//~ UCL_ResourceComponent Begin
public:
	virtual float GetValue() const override { return GetHealth(); }
	virtual void SetValue(float NewValue) override { SetHealth(NewValue); }
	virtual FGameplayAttribute GetValueAttribute() const override { return GetHealthAttribute(); }
	virtual float GetMaxValue() const override { return GetMaxHealth(); }
	virtual void SetMaxValue(float NewValue) override { SetMaxHealth(NewValue); }
	virtual FGameplayAttribute GetMaxValueAttribute() const override { return GetMaxHealthAttribute(); }
	//~ UCL_ResourceComponent End
};
