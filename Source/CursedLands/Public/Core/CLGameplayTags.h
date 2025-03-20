// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FCLGameplayTags
{
	static const FCLGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	// Secondary Attributes
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxStamina;

	// Vital Attributes
	FGameplayTag Attributes_Vital_Health;
	FGameplayTag Attributes_Vital_Stamina;

	// Locomotion States
	FGameplayTag Locomotion_Sprinting;

	// Debuffs
	FGameplayTag Debuff_Fatigue;

private:
	static FCLGameplayTags GameplayTags; 
};
