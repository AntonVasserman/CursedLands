// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/Gameplay/AbilitySystem/Abilities/CL_GameplayAbility_SprintToggle.h"

#include "Characters/CL_PlayerCharacter.h"

//~ Begin UCL_GameplayAbility_ToggleBase

bool UCL_GameplayAbility_SprintToggle::IsToggled(const ACL_PlayerCharacter* PlayerCharacter) const
{
	return PlayerCharacter->IsSprinting();
}

bool UCL_GameplayAbility_SprintToggle::CanToggle(const ACL_PlayerCharacter* PlayerCharacter) const
{
	return PlayerCharacter->CanSprint();
}

bool UCL_GameplayAbility_SprintToggle::CanUnToggle(const ACL_PlayerCharacter* PlayerCharacter) const
{
	return true;
}

void UCL_GameplayAbility_SprintToggle::Toggle(ACL_PlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->Sprint();
}

void UCL_GameplayAbility_SprintToggle::UnToggle(ACL_PlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->UnSprint();
}

//~ End UCL_GameplayAbility_ToggleBase
