// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/Gameplay/AbilitySystem/Abilities/CL_GameplayAbility_ToggleBase.h"

#include "CL_LogChannels.h"
#include "Characters/CL_PlayerCharacter.h"

//~ Begin UCL_GameplayAbility_PlayerCharacterBase

bool UCL_GameplayAbility_ToggleBase::CanActivateAbilityInternal(const ACL_PlayerCharacter* PlayerCharacter, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return (IsToggled(PlayerCharacter) && CanUnToggle(PlayerCharacter)) || CanToggle(PlayerCharacter);
}

void UCL_GameplayAbility_ToggleBase::ActivateAbilityInternal(ACL_PlayerCharacter* PlayerCharacter, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
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

//~ End UCL_GameplayAbility_PlayerCharacterBase
