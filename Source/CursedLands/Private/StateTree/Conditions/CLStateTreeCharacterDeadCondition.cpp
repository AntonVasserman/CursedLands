// Copyright Anton Vasserman, All Rights Reserved.

#include "StateTree/Conditions/CLStateTreeCharacterDeadCondition.h"

#include "StateTreeExecutionContext.h"
#include "Characters/CLCharacter.h"
#include "Controllers/CLPlayerController.h"

bool FCL_StateTreeCharacterDeadCondition::TestCondition(FStateTreeExecutionContext& Context) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	const ACLCharacter* Character = CastChecked<ACLCharacter>(InstanceData.Controller->GetCharacter());
	return Character->HasMatchingGameplayTag(CLGameplayTags::Status_Dead);
}
