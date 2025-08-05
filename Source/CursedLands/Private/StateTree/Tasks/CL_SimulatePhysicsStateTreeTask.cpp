// Copyright Anton Vasserman, All Rights Reserved.

#include "StateTree/Tasks/CL_SimulatePhysicsStateTreeTask.h"

#include "AV_CollisionProfileStatics.h"
#include "StateTreeExecutionContext.h"
#include "Characters/CL_Character.h"
#include "Controllers/CL_PlayerController.h"

EStateTreeRunStatus FCL_SimulatePhysicsStateTreeTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	
	InstanceData.Character->GetMesh()->SetSimulatePhysics(true);
	InstanceData.Character->GetMesh()->SetCollisionProfileName(UAV_CollisionProfileStatics::Ragdoll_ProfileName);
	
	return EStateTreeRunStatus::Succeeded;
}
