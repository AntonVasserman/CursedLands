// Copyright Anton Vasserman, All Rights Reserved.

#include "StateTree/Conditions/CLControllerCharacterHasMatchingTagCondition.h"

#include "StateTreeExecutionContext.h"
#include "Characters/CLCharacter.h"
#include "Controllers/CLPlayerController.h"

bool FCL_ControllerCharacterHasMatchingTagCondition::TestCondition(FStateTreeExecutionContext& Context) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	const ACLCharacter* Character = CastChecked<ACLCharacter>(InstanceData.Controller->GetCharacter());
	return Character->HasMatchingGameplayTag(InstanceData.Tag);
}
