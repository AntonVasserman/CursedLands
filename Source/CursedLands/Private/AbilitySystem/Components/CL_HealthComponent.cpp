// Copyright Anton Vasserman, All Rights Reserved.

#include "AbilitySystem/Components/CL_HealthComponent.h"

#include "CL_GameplayTags.h"
#include "CL_LogChannels.h"
#include "AbilitySystem/CL_AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/CL_HealthAttributeSet.h"

UCL_HealthComponent::UCL_HealthComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	ResourceAttributeSetClass = UCL_HealthAttributeSet::StaticClass();
	ResourceGameplayTags.Full = CLGameplayTags::Resource_Health_Full;
	ResourceGameplayTags.Normal = CLGameplayTags::Resource_Health_Normal;
	ResourceGameplayTags.Critical = CLGameplayTags::Resource_Health_Critical;
	ResourceGameplayTags.Depleted = CLGameplayTags::Resource_Health_Depleted;
}

bool UCL_HealthComponent::IsAlive() const
{
	if (AbilitySystemComponent == nullptr)
	{
		UE_LOG(LogCLGameplayAbilitySystem, Warning, TEXT("CL_HealthComponent::%hs: AbilitySystemComponent is not yet initialized."), __FUNCTION__);
		return false;
	}
	
	return AbilitySystemComponent->HasMatchingGameplayTag(CLGameplayTags::Status_Alive);
}

void UCL_HealthComponent::Die() const
{
	AbilitySystemComponent->SetLooseGameplayTagCount(CLGameplayTags::Status_Alive, 0);
	AbilitySystemComponent->SetLooseGameplayTagCount(CLGameplayTags::Status_Dead, 1);
}

void UCL_HealthComponent::InitializeWithAbilitySystem(UCL_AbilitySystemComponent* InAbilitySystemComponent)
{
	Super::InitializeWithAbilitySystem(InAbilitySystemComponent);

	// In initialization the health should be full and the character should be alive
	AbilitySystemComponent->SetLooseGameplayTagCount(CLGameplayTags::Status_Alive, 1);
}

//~ UCL_ResourceComponent Begin

void UCL_HealthComponent::ResourceStateChanged(ECL_ResourceState OldState, ECL_ResourceState NewState)
{
	if (NewState == ECL_ResourceState::Depleted)
	{
		if (IsAlive())
		{
			// Health depleted meaning the character is dead
			Die();
		}
	}
}

//~ UCL_ResourceComponent End
