// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLAttributeSet.h"
#include "CLStaminaAttributeSet.generated.h"

UCLASS()
class CURSEDLANDS_API UCLStaminaAttributeSet : public UCLAttributeSet
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
	
	//~ UCLAttributeSet Begin
protected:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//~ UCLAttributeSet End
};
