// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/Gameplay/AbilitySystem/Abilities/CL_GameplayAbility_Slide.h"

#include "CL_LogChannels.h"
#include "Characters/CL_PlayerCharacter.h"

//~ Begin UCL_GameplayAbility

bool UCL_GameplayAbility_Slide::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags) == false)
	{
		return false;
	}

	if (ActorInfo == nullptr || ActorInfo->AvatarActor.IsValid() == false)
	{
		CL_LOG_GAMEPLAY_ABILITY_SYSTEM(Warning, "Cannot activate ability - ActorInfo is null or AvatarActor is invalid");
		return false;
	}

	const ACL_PlayerCharacter* PlayerCharacter = CastChecked<ACL_PlayerCharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	if (PlayerCharacter == nullptr)
	{
		CL_LOG_GAMEPLAY_ABILITY_SYSTEM(Warning, "Cannot activate ability - AvatarActor is not of type ACL_PlayerCharacter");
		return false;
	}

	return PlayerCharacter->CanSlide();
}

void UCL_GameplayAbility_Slide::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ACL_PlayerCharacter* PlayerCharacter = CastChecked<ACL_PlayerCharacter>(ActorInfo->AvatarActor.Get());

	CommitAbility(Handle, ActorInfo, ActivationInfo);
	PlayerCharacter->Slide();
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

//~ End UCL_GameplayAbility 
