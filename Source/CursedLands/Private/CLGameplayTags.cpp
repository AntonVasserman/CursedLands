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
	
	// Secondary Attributes
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("Maximum amount of Health obtainable"));
	GameplayTags.Attributes_Secondary_MaxStamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxStamina"),
		FString("Maximum amount of Stamina obtainable"));

	// Vital Attributes
	GameplayTags.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.Health"),
		FString("A resource used to indicate amount of damage a character can receive"));
	GameplayTags.Attributes_Vital_Stamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.Stamina"),
		FString("A resource used to do actions such as sprint and cast spells"));

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

