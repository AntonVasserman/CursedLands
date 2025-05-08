// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

enum class ECLTraversalAction : uint8;
enum class ECLStance : uint8;
enum class ECLGait : uint8;

namespace CLGameplayTags
{
	// Statuses

	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Alive);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Dead);
	
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

	// Custom Movement Modes Tags
	CURSEDLANDS_API extern const TMap<uint8, FGameplayTag> CustomMovementModeTagMap;

	// Locomotion Stance Tags
	CURSEDLANDS_API extern const TMap<ECLStance, FGameplayTag> StanceTagMap;
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Locomotion_Stance_Standing);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Locomotion_Stance_Crouching);
	
	// Locomotion Gait Tags
	CURSEDLANDS_API extern const TMap<ECLGait, FGameplayTag> GaitTagMap;
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Locomotion_Gait_Walking);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Locomotion_Gait_Jogging);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Locomotion_Gait_Sprinting);
	
	// Locomotion Tags
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Locomotion_Rolling);

	// Traversal Tags
	CURSEDLANDS_API extern const TMap<ECLTraversalAction, FGameplayTag> TraversalActionTagMap;
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Traversal_Action_Hurdle);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Traversal_Action_Mantle);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Traversal_Action_Vault);
	CURSEDLANDS_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Traversal_Action_Slide);
}
