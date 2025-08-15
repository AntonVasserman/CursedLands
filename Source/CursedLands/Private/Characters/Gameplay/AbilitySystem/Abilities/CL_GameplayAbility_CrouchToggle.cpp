// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/Gameplay/AbilitySystem/Abilities/CL_GameplayAbility_CrouchToggle.h"

#include "Characters/CL_PlayerCharacter.h"

//~ Begin UCL_GameplayAbility_ToggleBase

bool UCL_GameplayAbility_CrouchToggle::IsToggled(const ACL_PlayerCharacter* PlayerCharacter) const
{
	return PlayerCharacter->IsCrouching();
}

bool UCL_GameplayAbility_CrouchToggle::CanToggle(const ACL_PlayerCharacter* PlayerCharacter) const
{
	return PlayerCharacter->IsSprinting() == false && PlayerCharacter->CanCrouch();
}

bool UCL_GameplayAbility_CrouchToggle::CanUnToggle(const ACL_PlayerCharacter* PlayerCharacter) const
{
	return true;
}

void UCL_GameplayAbility_CrouchToggle::Toggle(ACL_PlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->Crouch();
}

void UCL_GameplayAbility_CrouchToggle::UnToggle(ACL_PlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->UnCrouch();
}

//~ End UCL_GameplayAbility_ToggleBase
