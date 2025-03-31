// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLAttributeSet.h"
#include "CLHealthAttributeSet.generated.h"

UCLASS()
class CURSEDLANDS_API UCLHealthAttributeSet : public UCLAttributeSet
{
	GENERATED_BODY()

public:
	UCLHealthAttributeSet();

	ATTRIBUTE_ACCESSORS(UCLHealthAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UCLHealthAttributeSet, MaxHealth);
	
private:
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Ability System | Attributes", Meta = (AllowPrivateAccess))
	FGameplayAttributeData Health;
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay Ability System | Attributes", Meta = (AllowPrivateAccess))
	FGameplayAttributeData MaxHealth;

	//~ UCLAttributeSet Begin
protected:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//~ UCLAttributeSet End
};
