// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FCLGameplayTags
{
	static const FCLGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxStamina;
	
	FGameplayTag Attributes_Vital_Health;
	FGameplayTag Attributes_Vital_Stamina;

private:
	static FCLGameplayTags GameplayTags; 
};
