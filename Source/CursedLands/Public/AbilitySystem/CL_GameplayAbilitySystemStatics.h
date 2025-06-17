// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_GameplayAbilitySystemStatics.generated.h"

class UAbilitySystemComponent;

UCLASS(MinimalAPI)
class UCL_GameplayAbilitySystemStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System")
	static TOptional<UAbilitySystemComponent*> GetAbilitySystemComponent(const ACharacter* Character);
};
