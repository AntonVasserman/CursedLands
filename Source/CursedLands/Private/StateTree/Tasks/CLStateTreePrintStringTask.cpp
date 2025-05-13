// Copyright Anton Vasserman, All Rights Reserved.

#include "StateTree/Tasks/CLStateTreePrintStringTask.h"
#include "StateTreeExecutionContext.h"

static TAutoConsoleVariable CVarShowDebugCLStateTreePrintStringTask(
	TEXT("CLShowDebug.StateTreePrintStringTask"),
	false,
	TEXT("Shows the strings printed by the StateTreePrintStringTask class"),
	ECVF_Default);

EStateTreeRunStatus FCL_StateTreePrintStringTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	
	if (!GEngine)
	{
		return InstanceData.bRunForever ? EStateTreeRunStatus::Running : EStateTreeRunStatus::Failed;
	}

	if (CVarShowDebugCLStateTreePrintStringTask->GetBool())
	{
		const int32 Key = static_cast<int32>(GetTypeHash(InstanceData.StringToPrint));
		GEngine->AddOnScreenDebugMessage(Key, 1.f, FColor::Cyan, InstanceData.StringToPrint);	
	}
	
	return InstanceData.bRunForever ? EStateTreeRunStatus::Running : EStateTreeRunStatus::Succeeded;
}
