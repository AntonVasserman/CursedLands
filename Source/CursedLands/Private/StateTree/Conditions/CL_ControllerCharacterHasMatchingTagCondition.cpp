// Copyright Anton Vasserman, All Rights Reserved.

#include "StateTree/Conditions/CL_ControllerCharacterHasMatchingTagCondition.h"

#include "StateTreeExecutionContext.h"
#include "Characters/CL_Character.h"
#include "Controllers/CL_PlayerController.h"

bool FCL_ControllerCharacterHasMatchingTagCondition::TestCondition(FStateTreeExecutionContext& Context) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	const ACL_Character* Character = CastChecked<ACL_Character>(InstanceData.Controller->GetCharacter());
	return Character->HasMatchingGameplayTag(InstanceData.Tag);
}
