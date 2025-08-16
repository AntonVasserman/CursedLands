// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Abilities/CL_GameplayAbility.h"

#include "Characters/CL_Character.h"
#include "Characters/CL_PlayerCharacter.h"

ACL_Character* UCL_GameplayAbility::GetCLCharacterFromActorInfo() const
{
	return CurrentActorInfo ? Cast<ACL_Character>(CurrentActorInfo->AvatarActor.Get()) : nullptr;
}

ACL_PlayerCharacter* UCL_GameplayAbility::GetCLPlayerCharacterFromActorInfo() const
{
	return CurrentActorInfo ? Cast<ACL_PlayerCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr;
}
