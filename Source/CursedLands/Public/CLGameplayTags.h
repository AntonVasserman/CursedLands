// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

enum class ECLMovementWalkingMode : uint8;

namespace CLGameplayTags
{
	// Attributes
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes);

	// General Attributes
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_General_Health);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_General_MaxHealth);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_General_Mana);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_General_MaxMana);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_General_Stamina);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_General_MaxStamina);
	
	// Debuffs
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Fatigue);
	
	// Unreal Movement Modes Tags
	CURSEDLANDS_API extern const TMap<uint8, FGameplayTag> MovementModeTagMap;
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Walking);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_NavWalking);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Falling);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Swimming);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Flying);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Custom);
	
	// Movement Modes Sub-Tags
	CURSEDLANDS_API	extern const TSet<uint8> MovementModesUsingSubTags;

	// Movement Walking-Mode Sub-Tags
	CURSEDLANDS_API extern const TMap<ECLMovementWalkingMode, FGameplayTag> MovementWalkingModeTagMap;
	
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Walking_TODO);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Walking_Idle);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Walking_Jogging);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Walking_Sprinting);

	// Custom Movement Modes Tags
	CURSEDLANDS_API extern const TMap<uint8, FGameplayTag> CustomMovementModeTagMap;
	
	// Locomotion States
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Locomotion_Rolling);
}
