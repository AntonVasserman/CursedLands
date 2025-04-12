// Copyright Anton Vasserman, All Rights Reserved.


#include "CLGameplayTags.h"

#include "Engine/EngineTypes.h"
#include "Characters/CLCharacterMovementComponent.h"

namespace CLGameplayTags
{
	// Attributes
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes, "Attributes", "Base parent for all Attribute Gameplay Tags");
	
	// General Attributes
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_General_Health, "Attributes.General.Health", "A resource used to indicate amount of damage a character can receive");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_General_MaxHealth, "Attributes.General.MaxHealth", "Maximum amount of Health obtainable");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_General_Mana, "Attributes.General.Mana", "A resource used to do actions such as cast spells and buffs");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_General_MaxMana, "Attributes.General.MaxMana", "Maximum amount of Mana obtainable");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_General_Stamina, "Attributes.General.Stamina", "A resource used to do actions such as sprint and attack");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_General_MaxStamina, "Attributes.General.MaxStamina", "Maximum amount of Stamina obtainable");

	// Debuffs
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Fatigue, "Debuff.Fatigue", "Fatigue Debuff tag");
	
	// Unreal Movement Modes Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Walking, "Movement.Mode.Walking", "Default Character movement tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_NavWalking, "Movement.Mode.NavWalking", "Default Character movement tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Falling, "Movement.Mode.Falling", "Default Character movement tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Swimming, "Movement.Mode.Swimming", "Default Character movement tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Flying, "Movement.Mode.Flying", "Default Character movement tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Custom, "Movement.Mode.Custom", "This should be replaced with custom tags");

	const TMap<uint8, FGameplayTag> MovementModeTagMap =
	{
		{ MOVE_Walking, Movement_Mode_Walking },
		{ MOVE_NavWalking, Movement_Mode_NavWalking },
		{ MOVE_Falling, Movement_Mode_Falling },
		{ MOVE_Swimming, Movement_Mode_Swimming },
		{ MOVE_Flying, Movement_Mode_Flying },
		{ MOVE_Custom, Movement_Mode_Custom }
	};

	// Movement Modes Sub-Tags
	const TSet<uint8> MovementModesUsingSubTags =
	{
		MOVE_Walking
	};

	// Movement Walking-Mode Sub-Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Walking_TODO, "Movement.Mode.Walking.TODO", "THIS IS A PLACE HOLDER, SHOULD BE REMOVED IN CL-9")
	// TODO (CL-9): Implement walking states, Idle/Running...
	// UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Walking_Idle, "Movement.Mode.Walking.Idle", "Default Character movement sub-tag")
	// UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Walking_Running, "Movement.Mode.Walking.Running", "Default Character movement sub-tag")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Walking_Sprinting, "Movement.Mode.Walking.Sprinting", "Default Character movement sub-tag")
	
	const TMap<ECLMovementWalkingMode, FGameplayTag> MovementWalkingModeTagMap =
	{
		{ ECLMovementWalkingMode::TODO, Movement_Mode_Walking_TODO },
		// TODO (CL-9): Implement walking states, Idle/Running...
		// { ECLMovementWalkingMode::Idle, Movement_Mode_Walking_Idle },
		// { ECLMovementWalkingMode::Running, Movement_Mode_Walking_Running },
		{ ECLMovementWalkingMode::Sprinting, Movement_Mode_Walking_Sprinting },
	};;
	
	// Custom Movement Modes Tags
	const TMap<uint8, FGameplayTag> CustomMovementModeTagMap =
	{
		// Placeholder for Custom Movement Modes to Tags mapping
	};

	// Locomotion States
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Locomotion_Rolling, "Locomotion.Rolling", "Locomotion tag indicating rolling");
}
