// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FCLGameplayTags
{
	static const FCLGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	// Attributes
	FGameplayTag Attributes;

	// General Attributes
	FGameplayTag Attributes_General_Health;
	FGameplayTag Attributes_General_MaxHealth;
	FGameplayTag Attributes_General_Stamina;
	FGameplayTag Attributes_General_MaxStamina;
	
	// Locomotion States
	FGameplayTag Locomotion_Sprinting;

	// Debuffs
	FGameplayTag Debuff_Fatigue;

private:
	static FCLGameplayTags GameplayTags; 
};
