// Copyright Anton Vasserman, All Rights Reserved.


#include "CLGameplayTags.h"

#include "Engine/EngineTypes.h"
#include "Characters/CLCharacterMovementComponent.h"
#include "TraversalSystem/CLTraversalAction.h"

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

	// Custom Movement Modes Tags
	const TMap<uint8, FGameplayTag> CustomMovementModeTagMap =
	{
		// Placeholder for Custom Movement Modes to Tags mapping
	};

	// Locomotion Stance Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Locomotion_Stance_Standing, "Locomotion.Stance.Standing", "Stance tag")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Locomotion_Stance_Crouching, "Locomotion.Stance.Crouching", "Stance tag")
	const TMap<ECLStance, FGameplayTag> StanceTagMap =
	{
		{ ECLStance::Standing, Locomotion_Stance_Standing },
		{ ECLStance::Crouching, Locomotion_Stance_Crouching }
	};
	
	// Locomotion Gait Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Locomotion_Gait_Walking, "Locomotion.Gait.Walking", "Gait tag")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Locomotion_Gait_Jogging, "Locomotion.Gait.Jogging", "Gait tag")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Locomotion_Gait_Sprinting, "Locomotion.Gait.Sprinting", "Gait tag")
	const TMap<ECLGait, FGameplayTag> GaitTagMap =
	{
		{ ECLGait::Walking, Locomotion_Gait_Walking },
		{ ECLGait::Jogging, Locomotion_Gait_Jogging },
		{ ECLGait::Sprinting, Locomotion_Gait_Sprinting }
	};
	
	// Locomotion Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Locomotion_Rolling, "Locomotion.Rolling", "Locomotion tag indicating rolling");

	// Traversal Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Traversal_Action_Hurdle, "Traversal.Action.Hurdle", "Traversal action tag indicating a hurdle");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Traversal_Action_Mantle, "Traversal.Action.Mantle", "Traversal action tag indicating a mantle");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Traversal_Action_Vault, "Traversal.Action.Vault", "Traversal action tag indicating a vault");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Traversal_Action_Slide, "Traversal.Action.Slide", "Traversal action tag indicating a slide");
	const TMap<ECLTraversalAction, FGameplayTag> TraversalActionTagMap =
	{
		{ ECLTraversalAction::Hurdle, Traversal_Action_Hurdle },
		{ ECLTraversalAction::Mantle, Traversal_Action_Mantle },
		{ ECLTraversalAction::Vault, Traversal_Action_Vault },
		{ ECLTraversalAction::Slide, Traversal_Action_Slide }
	};
}
