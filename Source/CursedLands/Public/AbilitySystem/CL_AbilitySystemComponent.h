// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CL_AbilitySystemComponent.generated.h"

UCLASS()
class CURSEDLANDS_API UCL_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);

private:
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

	void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec);
	void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec);
	
	//~ UAbilitySystemComponent Begin
	//~ UAbilitySystemComponent End
};
