// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/Gameplay/AbilitySystem/Abilities/CL_GameplayAbility_ToggleBase.h"

#include "CL_LogChannels.h"
#include "Characters/CL_PlayerCharacter.h"

bool UCL_GameplayAbility_ToggleBase::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
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

	// We can toggle only if we are already toggled (to untoggle) or not toggled, but can toggle.
	return (IsToggled(PlayerCharacter) && CanUnToggle(PlayerCharacter)) || CanToggle(PlayerCharacter);
}

void UCL_GameplayAbility_ToggleBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// We shouldn't be able to reach here with an object that isn't ACL_PlayerCharacter
	ACL_PlayerCharacter* PlayerCharacter = CastChecked<ACL_PlayerCharacter>(ActorInfo->AvatarActor.Get());
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	if (IsToggled(PlayerCharacter))
	{
		UnToggle(PlayerCharacter);
	}
	else
	{
		Toggle(PlayerCharacter);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
