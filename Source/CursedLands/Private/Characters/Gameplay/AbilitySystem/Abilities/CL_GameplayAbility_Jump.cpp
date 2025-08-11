// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/Gameplay/AbilitySystem/Abilities/CL_GameplayAbility_Jump.h"

#include "CL_LogChannels.h"
#include "Characters/CL_Character.h"
#include "Characters/CL_PlayerCharacter.h"

//~ Begin UCL_GameplayAbility

bool UCL_GameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
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

	const ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	if (Character == nullptr)
	{
		CL_LOG_GAMEPLAY_ABILITY_SYSTEM(Warning, "Cannot activate ability - AvatarActor is not of type ACharacter");
		return false;
	}
	
	return Character->CanJump();
}

void UCL_GameplayAbility_Jump::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	// We shouldn't be able to get here with something that isn't ACharacter
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->StopJumping();
}

void UCL_GameplayAbility_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo == nullptr || ActorInfo->AvatarActor.IsValid() == false)
	{
		CL_LOG_GAMEPLAY_ABILITY_SYSTEM(Warning, "Input was Released but ActorInfo is null or AvatarActor is invalid");
		return;
	}

	CancelAbility(Handle, ActorInfo, ActivationInfo, false);
}

void UCL_GameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// We shouldn't be able to get here with something that isn't ACharacter
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->Jump();
}

//~ End UCL_GameplayAbility
