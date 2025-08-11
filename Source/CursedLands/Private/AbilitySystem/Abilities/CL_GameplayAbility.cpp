// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Abilities/CL_GameplayAbility.h"

#include "Characters/CL_Character.h"

ACL_Character* UCL_GameplayAbility::GetCLPlayerCharacterFromActorInfo() const
{
	return CurrentActorInfo ? Cast<ACL_Character>(CurrentActorInfo->AvatarActor.Get()) : nullptr;
}
