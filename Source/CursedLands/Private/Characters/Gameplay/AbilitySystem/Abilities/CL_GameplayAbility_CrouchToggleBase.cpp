// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/Gameplay/AbilitySystem/Abilities/CL_GameplayAbility_CrouchToggleBase.h"

#include "Characters/CL_PlayerCharacter.h"

//~ Begin UCL_GameplayAbility_ToggleBase

bool UCL_GameplayAbility_CrouchToggleBase::IsToggled(const ACL_PlayerCharacter* PlayerCharacter) const
{
	return PlayerCharacter->IsCrouching();
}

bool UCL_GameplayAbility_CrouchToggleBase::CanToggle(const ACL_PlayerCharacter* PlayerCharacter) const
{
	return PlayerCharacter->IsSprinting() == false && PlayerCharacter->CanCrouch();
}

bool UCL_GameplayAbility_CrouchToggleBase::CanUnToggle(const ACL_PlayerCharacter* PlayerCharacter) const
{
	return true;
}

void UCL_GameplayAbility_CrouchToggleBase::Toggle(ACL_PlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->Crouch();
}

void UCL_GameplayAbility_CrouchToggleBase::UnToggle(ACL_PlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->UnCrouch();
}

//~ End UCL_GameplayAbility_ToggleBase
