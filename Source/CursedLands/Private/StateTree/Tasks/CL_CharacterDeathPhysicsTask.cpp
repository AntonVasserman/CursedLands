// Copyright Anton Vasserman, All Rights Reserved.

#include "StateTree/Tasks/CL_CharacterDeathPhysicsTask.h"

#include "AVCollisionProfileStatics.h"
#include "StateTreeExecutionContext.h"
#include "Characters/CL_Character.h"
#include "Controllers/CL_PlayerController.h"

EStateTreeRunStatus FCL_CharacterDeathPhysicsTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	const ACL_Character* Character = CastChecked<ACL_Character>(InstanceData.Controller->GetCharacter());
	Character->GetMesh()->SetSimulatePhysics(true);
	Character->GetMesh()->SetCollisionProfileName(UAVCollisionProfileStatics::Ragdoll_ProfileName);
	
	return InstanceData.bRunForever ? EStateTreeRunStatus::Running : EStateTreeRunStatus::Succeeded;
}
