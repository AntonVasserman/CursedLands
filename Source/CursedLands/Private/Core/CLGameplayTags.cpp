// Copyright Anton Vasserman, All Rights Reserved.


#include "Core/CLGameplayTags.h"
#include "GameplayTagsManager.h"

FCLGameplayTags FCLGameplayTags::GameplayTags;

void FCLGameplayTags::InitializeNativeGameplayTags()
{
	// Secondary Attributes
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("Maximum amount of Health obtainable"));
	GameplayTags.Attributes_Secondary_MaxStamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxStamina"),
		FString("Maximum amount of Stamina obtainable"));

	// Vital Attributes
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.Health"),
		FString("A resource used to indicate amount of damage a character can receive"));
	GameplayTags.Attributes_Secondary_MaxStamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.Stamina"),
		FString("A resource used to do actions such as sprint and cast spells"));
}

