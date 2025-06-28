// Copyright Anton Vasserman, All Rights Reserved.

#include "AbilitySystem/Components/CL_StaminaComponent.h"

#include "CL_GameplayTags.h"
#include "CL_LogChannels.h"
#include "AbilitySystem/CL_AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/CL_StaminaAttributeSet.h"

UCL_StaminaComponent::UCL_StaminaComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	ResourceAttributeSetClass = UCL_StaminaAttributeSet::StaticClass();
	ResourceGameplayTags.Full = CLGameplayTags::Resource_Stamina_Full;
	ResourceGameplayTags.Normal = CLGameplayTags::Resource_Stamina_Normal;
	ResourceGameplayTags.Critical = CLGameplayTags::Resource_Stamina_Critical;
	ResourceGameplayTags.Depleted = CLGameplayTags::Resource_Stamina_Depleted;
}

bool UCL_StaminaComponent::IsFatigued() const
{
	if (AbilitySystemComponent == nullptr)
	{
		UE_LOG(LogCLGameplayAbilitySystem, Warning, TEXT("CL_StaminaComponent::%hs: AbilitySystemComponent is not yet initialized."), __FUNCTION__);
		return false;
	}
	
	return AbilitySystemComponent->HasMatchingGameplayTag(CLGameplayTags::Debuff_Fatigue);
}

void UCL_StaminaComponent::ApplyFatigue() const
{
	if (IsFatigued())
	{
		UE_LOG(LogCLGameplayAbilitySystem, Warning, TEXT("CL_StaminaComponent::%hs: %s is already Fatigued, re-apply isn't supported."), __FUNCTION__, *GetOwner()->GetFullName());
		return;
	}

	// On the contrary, if the user creates a task, he must assign a gameplay effect to be used with it
	checkf(FatigueGameplayEffectClass, TEXT("CL_StaminaComponent::%hs: FatigueGameplayEffectClass uninitialized in ApplyGameplayEffectLandedTask for Character: %s"), __FUNCTION__, *GetOwner()->GetFullName());

	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(FatigueGameplayEffectClass, FatigueGameplayEffectLevel, ContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), AbilitySystemComponent);
}

//~ UCL_ResourceComponent Begin

void UCL_StaminaComponent::ResourceStateChanged(ECL_ResourceState OldState, ECL_ResourceState NewState)
{
	if (NewState == ECL_ResourceState::Depleted)
	{
		if (bApplyFatigueOnStaminaDepleted)
		{
			ApplyFatigue();
		}
	}
}

//~ UCL_ResourceComponent End