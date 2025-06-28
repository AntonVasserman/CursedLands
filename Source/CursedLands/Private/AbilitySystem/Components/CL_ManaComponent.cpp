// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Components/CL_ManaComponent.h"

#include "CL_GameplayTags.h"
#include "AbilitySystem/Attributes/CL_ManaAttributeSet.h"

UCL_ManaComponent::UCL_ManaComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	ResourceAttributeSetClass = UCL_ManaAttributeSet::StaticClass();
	ResourceGameplayTags.Full = CLGameplayTags::Resource_Mana_Full;
	ResourceGameplayTags.Normal = CLGameplayTags::Resource_Mana_Normal;
	ResourceGameplayTags.Critical = CLGameplayTags::Resource_Mana_Critical;
	ResourceGameplayTags.Depleted = CLGameplayTags::Resource_Mana_Depleted;
}
