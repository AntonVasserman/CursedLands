// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

namespace CLGameplayTags
{
	// Movement Modes Tags
	CURSEDLANDS_API extern const TMap<uint8, FGameplayTag> MovementModeTagMap;
	CURSEDLANDS_API extern const TMap<uint8, FGameplayTag> CustomMovementModeTagMap;
	
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Walking);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_NavWalking);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Falling);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Swimming);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Flying);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Custom);

	// Custom Movement Modes Tags
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_CustomMode_Sprinting);
}

// TODO: Remove in favor of tag setup like in LyraGame
struct FCLGameplayTags
{
	static const FCLGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	// Attributes
	FGameplayTag Attributes;

	// General Attributes
	FGameplayTag Attributes_General_Health;
	FGameplayTag Attributes_General_MaxHealth;
	FGameplayTag Attributes_General_Mana;
	FGameplayTag Attributes_General_MaxMana;
	FGameplayTag Attributes_General_Stamina;
	FGameplayTag Attributes_General_MaxStamina;
	
	// Locomotion States
	FGameplayTag Locomotion_Rolling;

	// Debuffs
	FGameplayTag Debuff_Fatigue;

private:
	static FCLGameplayTags GameplayTags; 
};
