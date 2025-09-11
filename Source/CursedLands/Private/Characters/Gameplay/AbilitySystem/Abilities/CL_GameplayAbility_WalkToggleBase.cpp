// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/Gameplay/AbilitySystem/Abilities/CL_GameplayAbility_WalkToggleBase.h"

#include "Characters/CL_PlayerCharacter.h"

//~ Begin UCL_GameplayAbility_ToggleBase

bool UCL_GameplayAbility_WalkToggleBase::IsToggled(const ACL_PlayerCharacter* PlayerCharacter) const
{
	return PlayerCharacter->IsWalking();
}

bool UCL_GameplayAbility_WalkToggleBase::CanToggle(const ACL_PlayerCharacter* PlayerCharacter) const
{
	// In Crouching Stance the default is Walking, so we don't support toggling out of it
	return PlayerCharacter->CanWalk();
}

bool UCL_GameplayAbility_WalkToggleBase::CanUnToggle(const ACL_PlayerCharacter* PlayerCharacter) const
{
	return !PlayerCharacter->IsCrouching();
}

void UCL_GameplayAbility_WalkToggleBase::Toggle(ACL_PlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->Walk();
}

void UCL_GameplayAbility_WalkToggleBase::UnToggle(ACL_PlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->UnWalk();
}

//~ End UCL_GameplayAbility_ToggleBase
