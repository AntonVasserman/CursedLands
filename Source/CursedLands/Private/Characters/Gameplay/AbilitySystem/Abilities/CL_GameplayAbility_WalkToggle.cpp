// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/Gameplay/AbilitySystem/Abilities/CL_GameplayAbility_WalkToggle.h"

#include "Characters/CL_PlayerCharacter.h"

//~ Begin UCL_GameplayAbility_ToggleBase

bool UCL_GameplayAbility_WalkToggle::IsToggled(const ACL_PlayerCharacter* PlayerCharacter) const
{
	return PlayerCharacter->IsWalking();
}

bool UCL_GameplayAbility_WalkToggle::CanToggle(const ACL_PlayerCharacter* PlayerCharacter) const
{
	// In Crouching Stance the default is Walking, so we don't support toggling out of it
	return PlayerCharacter->CanWalk();
}

bool UCL_GameplayAbility_WalkToggle::CanUnToggle(const ACL_PlayerCharacter* PlayerCharacter) const
{
	return !PlayerCharacter->IsCrouching();
}

void UCL_GameplayAbility_WalkToggle::Toggle(ACL_PlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->Walk();
}

void UCL_GameplayAbility_WalkToggle::UnToggle(ACL_PlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->UnWalk();
}

//~ End UCL_GameplayAbility_ToggleBase
