// Copyright Anton Vasserman, All Rights Reserved.


#include "CLGameplayTags.h"

#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"
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

	// Custom Movement Modes Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_CustomMode_Sprinting, "Movement.CustomMode.Sprinting", "Custom Character movement tag");

	// Unreal Movement Modes
	const TMap<uint8, FGameplayTag> MovementModeTagMap =
	{
		{ MOVE_Walking, Movement_Mode_Walking },
		{ MOVE_NavWalking, Movement_Mode_NavWalking },
		{ MOVE_Falling, Movement_Mode_Falling },
		{ MOVE_Swimming, Movement_Mode_Swimming },
		{ MOVE_Flying, Movement_Mode_Flying },
		{ MOVE_Custom, Movement_Mode_Custom }
	};
	
	// Custom Movement Modes
	const TMap<uint8, FGameplayTag> CustomMovementModeTagMap =
	{
		{ CMOVE_Sprinting, Movement_CustomMode_Sprinting },
	};

	// Locomotion States
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Locomotion_Rolling, "Locomotion.Rolling", "Locomotion tag indicating rolling");
}
