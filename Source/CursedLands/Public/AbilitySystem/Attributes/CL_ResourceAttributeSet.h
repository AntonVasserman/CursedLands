// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_AttributeSet.h"
#include "CL_ResourceAttributeSet.generated.h"

UCLASS(Abstract)
class CURSEDLANDS_API UCL_ResourceAttributeSet : public UCL_AttributeSet
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System|Attribute|Resource")
	virtual float GetValue() const { return 0.0f; }

	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System|Attribute|Resource")
	virtual void SetValue(float NewValue) {}

	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System|Attribute|Resource")
	virtual FGameplayAttribute GetValueAttribute() const { return nullptr; }

	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System|Attribute|Resource")
	virtual float GetMaxValue() const { return 0.0f; }

	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System|Attribute|Resource")
	virtual void SetMaxValue(float NewValue) {}

	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System|Attribute|Resource")
	virtual FGameplayAttribute GetMaxValueAttribute() const { return nullptr; }
	
	//~ UCL_AttributeSet Begin
protected:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//~ UCL_AttributeSet End
};
