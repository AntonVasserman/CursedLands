// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/Gameplay/AbilitySystem/Abilities/CL_GameplayAbility_SprintToggleBase.h"

#include "Characters/CL_PlayerCharacter.h"

//~ Begin UCL_GameplayAbility_ToggleBase

bool UCL_GameplayAbility_SprintToggleBase::IsToggled(const ACL_PlayerCharacter* PlayerCharacter) const
{
	return PlayerCharacter->IsSprinting();
}

bool UCL_GameplayAbility_SprintToggleBase::CanToggle(const ACL_PlayerCharacter* PlayerCharacter) const
{
	return PlayerCharacter->CanSprint();
}

bool UCL_GameplayAbility_SprintToggleBase::CanUnToggle(const ACL_PlayerCharacter* PlayerCharacter) const
{
	return true;
}

void UCL_GameplayAbility_SprintToggleBase::Toggle(ACL_PlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->Sprint();
}

void UCL_GameplayAbility_SprintToggleBase::UnToggle(ACL_PlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->UnSprint();
}

//~ End UCL_GameplayAbility_ToggleBase
