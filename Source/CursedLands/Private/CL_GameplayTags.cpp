// Copyright Anton Vasserman, All Rights Reserved.


#include "CL_GameplayTags.h"

#include "Engine/EngineTypes.h"
#include "Characters/Components/CL_ExtendedCharacterMovementComponent.h"
#include "Characters/Fall/CL_FallType.h"
#include "Systems/Traversal/CL_TraversalAction.h"

namespace CLGameplayTags
{
	// Cheats
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cheat_Ability_WhiteMonster, "Cheat.Ability.WhiteMonster", "Cheat ability that grants infinite stamina for the duration of the tag");
	
	// UI Layers
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(UI_Layer_Game, "UI.Layer.Game", "UI Layer for the Game HUD");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(UI_Layer_PauseMenu, "UI.Layer.PauseMenu", "UI Layer for the Pause Menu");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(UI_Layer_Menu, "UI.Layer.Menu", "UI Layer for the Main Menu");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(UI_Layer_Modal, "UI.Layer.Modal", "UI Layer for Modals such as Pop-ups");
	
	// InputTags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move_Gamepad, "InputTag.Move.Gamepad", "InputTag for Move Action using Gamepad");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move_KeyboardAndMouse, "InputTag.Move.KeyboardAndMouse", "InputTag for Move Action using Keyboard and Mouse");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look, "InputTag.Look", "InputTag for Look Action");

	// Statuses
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Alive, "Status.Alive", "Character status tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Dead, "Status.Dead", "Character status tag");
	
	// Attributes
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes, "Attributes", "Base parent for all Attribute Gameplay Tags");
	
	// General Attributes
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_General_Health, "Attributes.General.Health", "A resource used to indicate amount of damage a character can receive");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_General_MaxHealth, "Attributes.General.MaxHealth", "Maximum amount of Health obtainable");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_General_Mana, "Attributes.General.Mana", "A resource used to do actions such as cast spells and buffs");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_General_MaxMana, "Attributes.General.MaxMana", "Maximum amount of Mana obtainable");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_General_Stamina, "Attributes.General.Stamina", "A resource used to do actions such as sprint and attack");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_General_MaxStamina, "Attributes.General.MaxStamina", "Maximum amount of Stamina obtainable");

	// Resource Attributes
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Resource_Health_Full, "Resource.Health.Full", "Health resource is full");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Resource_Health_Normal, "Resource.Health.Normal", "Health resource is at a normal amount");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Resource_Health_Critical, "Resource.Health.Critical", "Health resource is at a critical amount");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Resource_Health_Depleted, "Resource.Health.Depleted", "Health resource is depleted");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Resource_Health_Infinite, "Resource.Health.Infinite", "Health resource is infinite");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Resource_Mana_Full, "Resource.Mana.Full", "Mana resource is full");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Resource_Mana_Normal, "Resource.Mana.Normal", "Mana resource is at a normal amount");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Resource_Mana_Critical, "Resource.Mana.Critical", "Mana resource is at a critical amount");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Resource_Mana_Depleted, "Resource.Mana.Depleted", "Mana resource is depleted");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Resource_Mana_Infinite, "Resource.Mana.Infinite", "Mana resource is infinite");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Resource_Stamina_Full, "Resource.Stamina.Full", "Stamina resource is full");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Resource_Stamina_Normal, "Resource.Stamina.Normal", "Stamina resource is at a normal amount");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Resource_Stamina_Critical, "Resource.Stamina.Critical", "Stamina resource is at a critical amount");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Resource_Stamina_Depleted, "Resource.Stamina.Depleted", "Stamina resource is depleted");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Resource_Stamina_Infinite, "Resource.Stamina.Infinite", "Stamina resource is infinite");
	
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
	const TMap<ECL_Stance, FGameplayTag> StanceTagMap =
	{
		{ ECL_Stance::Standing, Locomotion_Stance_Standing },
		{ ECL_Stance::Crouching, Locomotion_Stance_Crouching }
	};
	
	// Locomotion Gait Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Locomotion_Gait_Walking, "Locomotion.Gait.Walking", "Gait tag")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Locomotion_Gait_Jogging, "Locomotion.Gait.Jogging", "Gait tag")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Locomotion_Gait_Sprinting, "Locomotion.Gait.Sprinting", "Gait tag")
	const TMap<ECL_Gait, FGameplayTag> GaitTagMap =
	{
		{ ECL_Gait::Walking, Locomotion_Gait_Walking },
		{ ECL_Gait::Jogging, Locomotion_Gait_Jogging },
		{ ECL_Gait::Sprinting, Locomotion_Gait_Sprinting }
	};
	
	// Locomotion Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Locomotion_Rolling, "Locomotion.Rolling", "Locomotion tag indicating rolling");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Locomotion_StandingUp, "Locomotion.StandingUp", "Locomotion tag indicating standing up from fall");

	// Falling Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Falling_Light, "Falling.Light", "Falling tag indicating light fall");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Falling_Medium, "Falling.Medium", "Falling tag indicating medium fall");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Falling_Deadly, "Falling.Deadly", "Falling tag indicating deadly fall");
	const TMap<FGameplayTag, ECL_FallType> TagToFallingMap =
	{
		{ Falling_Light, ECL_FallType::Light },
		{ Falling_Medium, ECL_FallType::Medium },
		{ Falling_Deadly, ECL_FallType::Deadly },
	};
	
	// Traversal Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Traversal_Action_Hurdle, "Traversal.Action.Hurdle", "Traversal action tag indicating a hurdle");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Traversal_Action_Mantle, "Traversal.Action.Mantle", "Traversal action tag indicating a mantle");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Traversal_Action_Vault, "Traversal.Action.Vault", "Traversal action tag indicating a vault");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Traversal_Action_Slide, "Traversal.Action.Slide", "Traversal action tag indicating a slide");
	const TMap<ECL_TraversalAction, FGameplayTag> TraversalActionTagMap =
	{
		{ ECL_TraversalAction::Hurdle, Traversal_Action_Hurdle },
		{ ECL_TraversalAction::Mantle, Traversal_Action_Mantle },
		{ ECL_TraversalAction::Vault, Traversal_Action_Vault },
		{ ECL_TraversalAction::Slide, Traversal_Action_Slide }
	};

	// State Tree Events
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(StateTree_Events_ReevaluateCameraRig, "StateTree.Events.ReevaluateCameraRig", "Indicates that we need to re-evaluate the CameraRig in the Gameplay Camera");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(StateTree_Events_Landed, "StateTree.Events.Landed", "Indicates that the Actor just landed after being in a falling state");
}
