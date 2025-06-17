// Copyright Anton Vasserman, All Rights Reserved.

#include "AbilitySystem/Components/CL_HealthComponent.h"

#include "CLGameplayTags.h"
#include "CLLogChannels.h"
#include "AbilitySystem/CL_AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/CL_HealthAttributeSet.h"

UCL_HealthComponent::UCL_HealthComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	ResourceAttributeSetClass = UCL_HealthAttributeSet::StaticClass();
}

TOptional<bool> UCL_HealthComponent::IsAlive() const
{
	if (AbilitySystemComponent == nullptr)
	{
		UE_LOG(LogCL, Warning, TEXT("CL_HealthComponent::%hs: AbilitySystemComponent is not yet initialized."), __FUNCTION__);
		return TOptional<bool>();
	}
	
	return AbilitySystemComponent->HasMatchingGameplayTag(CLGameplayTags::Status_Alive);
}

void UCL_HealthComponent::InitializeWithAbilitySystem(UCL_AbilitySystemComponent* InAbilitySystemComponent)
{
	Super::InitializeWithAbilitySystem(InAbilitySystemComponent);

	// In initialization the health should be full and the character should be alive
	AbilitySystemComponent->SetLooseGameplayTagCount(CLGameplayTags::Status_Alive, 1);
}

void UCL_HealthComponent::ResourceDepletedInternal()
{
	if (IsAlive())
	{
		// Health depleted meaning the character is dead
		AbilitySystemComponent->SetLooseGameplayTagCount(CLGameplayTags::Status_Alive, 0);
		AbilitySystemComponent->SetLooseGameplayTagCount(CLGameplayTags::Status_Dead, 1);	
	}
}

void UCL_HealthComponent::ResourceFullInternal()
{
}
