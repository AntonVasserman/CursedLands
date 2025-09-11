// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/Gameplay/AbilitySystem/Abilities/CL_GameplayAbility_TraverseBase.h"

#include "Characters/CL_PlayerCharacter.h"

//~ Begin UCL_GameplayAbility_PlayerCharacterBase

bool UCL_GameplayAbility_TraverseBase::CanActivateAbilityInternal(const ACL_PlayerCharacter* PlayerCharacter, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return PlayerCharacter->CanTraverse();
}

void UCL_GameplayAbility_TraverseBase::ActivateAbilityInternal(ACL_PlayerCharacter* PlayerCharacter, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	PlayerCharacter->Traverse();
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

//~ End UCL_GameplayAbility_PlayerCharacterBase
