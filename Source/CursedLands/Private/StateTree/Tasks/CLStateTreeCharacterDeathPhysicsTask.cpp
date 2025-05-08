// Copyright Anton Vasserman, All Rights Reserved.

#include "StateTree/Tasks/CLStateTreeCharacterDeathPhysicsTask.h"

#include "AVCollisionProfileStatics.h"
#include "StateTreeExecutionContext.h"
#include "Characters/CLCharacter.h"
#include "Controllers/CLPlayerController.h"

EStateTreeRunStatus FCL_StateTreeCharacterDeathPhysicsTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	// TODO: Consider a timer of 0.1 seconds here?

	const ACLCharacter* Character = CastChecked<ACLCharacter>(InstanceData.Controller->GetCharacter());
	Character->GetMesh()->SetSimulatePhysics(true);
	Character->GetMesh()->SetCollisionProfileName(UAVCollisionProfileStatics::Ragdoll_ProfileName);
	
	return InstanceData.bRunForever ? EStateTreeRunStatus::Running : EStateTreeRunStatus::Succeeded;
}
