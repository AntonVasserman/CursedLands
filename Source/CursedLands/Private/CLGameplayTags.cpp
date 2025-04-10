// Copyright Anton Vasserman, All Rights Reserved.


#include "CLGameplayTags.h"

#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"
#include "Characters/CLCharacterMovementComponent.h"

namespace CLGameplayTags
{
	// Movement Mode Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Walking, "Movement.Mode.Walking", "Default Character movement tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_NavWalking, "Movement.Mode.NavWalking", "Default Character movement tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Falling, "Movement.Mode.Falling", "Default Character movement tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Swimming, "Movement.Mode.Swimming", "Default Character movement tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Flying, "Movement.Mode.Flying", "Default Character movement tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Custom, "Movement.Mode.Custom", "This should be replaced with custom tags");

	// Custom Movement Mode Tags
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
}

FCLGameplayTags FCLGameplayTags::GameplayTags;

void FCLGameplayTags::InitializeNativeGameplayTags()
{
	// Attributes
	GameplayTags.Attributes = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attributes"),
			FString("The base parent for all Attribute Gameplay Tags"));

	// General Attributes
	GameplayTags.Attributes_General_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.General.Health"),
		FString("A resource used to indicate amount of damage a character can receive"));
	GameplayTags.Attributes_General_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.General.MaxHealth"),
		FString("Maximum amount of Health obtainable"));
	GameplayTags.Attributes_General_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attributes.General.Mana"),
			FString("A resource used to do actions such as cast spells and buffs"));
	GameplayTags.Attributes_General_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.General.MaxMana"),
		FString("Maximum amount of Mana obtainable"));
	GameplayTags.Attributes_General_Stamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.General.Stamina"),
		FString("A resource used to do actions such as sprint and attack"));
	GameplayTags.Attributes_General_MaxStamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.General.MaxStamina"),
		FString("Maximum amount of Stamina obtainable"));
	
	// Locomotion States
	GameplayTags.Locomotion_Rolling = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Locomotion.Rolling"),
		FString("The tag owner is marked as in rolling locomotion state."));

	// Debuffs
	// Debuff_Fatigue
	GameplayTags.Debuff_Fatigue = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Fatigue"),
		FString("The tag owner is marked as fatigued."));
}

