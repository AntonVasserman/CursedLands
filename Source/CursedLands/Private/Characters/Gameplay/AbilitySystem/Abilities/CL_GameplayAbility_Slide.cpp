// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/Gameplay/AbilitySystem/Abilities/CL_GameplayAbility_Slide.h"

#include "Characters/CL_PlayerCharacter.h"

//~ Begin UCL_GameplayAbility_PlayerCharacterBase

bool UCL_GameplayAbility_Slide::CanActivateAbilityInternal(const ACL_PlayerCharacter* PlayerCharacter, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return PlayerCharacter->CanSlide();
}

void UCL_GameplayAbility_Slide::ActivateAbilityInternal(ACL_PlayerCharacter* PlayerCharacter, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	CommitAbility(Handle, ActorInfo, ActivationInfo);
	PlayerCharacter->Slide();
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

//~ End UCL_GameplayAbility_PlayerCharacterBase 
