// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Abilities/CL_GameplayAbility.h"

#include "Characters/CL_Character.h"
#include "Characters/CL_PlayerCharacter.h"

ACL_Character* UCL_GameplayAbility::GetCLCharacterFromActorInfo(const FGameplayAbilityActorInfo* ActorInfo) const
{
	return ActorInfo ? Cast<ACL_Character>(ActorInfo->AvatarActor.Get()) : nullptr;
}

ACL_PlayerCharacter* UCL_GameplayAbility::GetCLPlayerCharacterFromActorInfo(const FGameplayAbilityActorInfo* ActorInfo) const
{
	return ActorInfo ? Cast<ACL_PlayerCharacter>(ActorInfo->AvatarActor.Get()) : nullptr;
}
