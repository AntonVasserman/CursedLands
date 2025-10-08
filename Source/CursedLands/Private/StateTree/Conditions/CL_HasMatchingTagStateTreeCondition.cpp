// Copyright Anton Vasserman, All Rights Reserved.

#include "StateTree/Conditions/CL_HasMatchingTagStateTreeCondition.h"

#include "GameplayTagAssetInterface.h"
#include "StateTreeExecutionContext.h"

bool FCL_HasMatchingTagStateTreeCondition::TestCondition(FStateTreeExecutionContext& Context) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	const IGameplayTagAssetInterface* GameplayTagAssetInterface = Cast<IGameplayTagAssetInterface>(InstanceData.Actor);
	
	return GameplayTagAssetInterface->HasMatchingGameplayTag(InstanceData.Tag) ? !InstanceData.bInvert : InstanceData.bInvert;
}
