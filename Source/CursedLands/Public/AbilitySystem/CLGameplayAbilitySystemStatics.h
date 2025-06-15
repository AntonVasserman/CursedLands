// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLGameplayAbilitySystemStatics.generated.h"

class UAbilitySystemComponent;

UCLASS(MinimalAPI)
class UCLGameplayAbilitySystemStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System")
	static TOptional<UAbilitySystemComponent*> GetAbilitySystemComponent(const ACharacter* Character);
};
