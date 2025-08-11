// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/Gameplay/Camera/CL_GetPlayerCharacterCameraInfoGCSTTask.h"

#include "CL_LogChannels.h"
#include "StateTreeExecutionContext.h"
#include "Characters/CL_PlayerCharacter.h"
#include "GameFramework/GameplayCameraComponent.h"

//~ FGameplayCamerasStateTreeTask Begin

EStateTreeRunStatus FCL_GetPlayerCharacterCameraInfoGCSTTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FGameplayCamerasStateTreeTask::EnterState(Context, Transition);

	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	InstanceData.PlayerCharacterRef = Cast<ACL_PlayerCharacter>(Cast<UGameplayCameraComponent>(Context.GetOwner())->GetOwner());
	if (!InstanceData.PlayerCharacterRef)
	{
		CL_LOG(Error, "Failed to get PlayerCharacterRef from StateTreeExecutionContext");
		return EStateTreeRunStatus::Failed;
	}
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FCL_GetPlayerCharacterCameraInfoGCSTTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FGameplayCamerasStateTreeTask::Tick(Context, DeltaTime);

	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (!InstanceData.PlayerCharacterRef)
	{
		return EStateTreeRunStatus::Failed;
	}

	InstanceData.Stance = InstanceData.PlayerCharacterRef->GetCLCharacterMovement()->GetStance();
	InstanceData.CameraMode = InstanceData.PlayerCharacterRef->GetCameraMode();
	InstanceData.bSprinting = InstanceData.PlayerCharacterRef->IsSprinting();
	
	return EStateTreeRunStatus::Running;
}

//~ FGameplayCamerasStateTreeTask End
