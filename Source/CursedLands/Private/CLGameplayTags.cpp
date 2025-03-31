// Copyright Anton Vasserman, All Rights Reserved.


#include "CLGameplayTags.h"
#include "GameplayTagsManager.h"

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
	GameplayTags.Locomotion_Sprinting = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Locomotion.Sprinting"),
		FString("The tag owner is marked as in sprinting locomotion state."));

	// Debuffs
	// Debuff_Fatigue
	GameplayTags.Debuff_Fatigue = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Fatigue"),
		FString("The tag owner is marked as fatigued."));
}

