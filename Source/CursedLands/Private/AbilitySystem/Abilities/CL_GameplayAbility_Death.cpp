// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Abilities/CL_GameplayAbility_Death.h"

#include "CL_GameplayTags.h"
#include "AbilitySystem/CL_AbilitySystemComponent.h"

UCL_GameplayAbility_Death::UCL_GameplayAbility_Death(const FObjectInitializer& ObjectInitializer)
{
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag to the CDO
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = CLGameplayTags::Status_Dead;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::OwnedTagAdded;
		AbilityTriggers.Add(TriggerData);
	}
}

//~ Begin UCL_GameplayAbility

void UCL_GameplayAbility_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	check(ActorInfo);

	UCL_AbilitySystemComponent* ASC = CastChecked<UCL_AbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());

	// Add abilities to allow using even when dead here...
	FGameplayTagContainer AbilitiesToIgnore;

	// Cancel and block all other abilities
	ASC->CancelAbilities(nullptr, &AbilitiesToIgnore, this);

	// Block all abilities from being activated
	FGameplayTagContainer BlockTags;
	BlockTags.AddTag(CLGameplayTags::InputTag_Ability);
	ASC->BlockAbilitiesWithTags(BlockTags);
	
	SetCanBeCanceled(false);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UCL_GameplayAbility_Death::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	check(ActorInfo);

	UCL_AbilitySystemComponent* ASC = CastChecked<UCL_AbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
	
	// Unblock abilities when death ability ends
	FGameplayTagContainer BlockTags;
	BlockTags.AddTag(CLGameplayTags::InputTag_Ability);
	ASC->UnBlockAbilitiesWithTags(BlockTags);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

//~ End UCL_GameplayAbility
