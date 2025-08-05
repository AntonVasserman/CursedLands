// Copyright Anton Vasserman, All Rights Reserved.


#include "StateTree/Tasks/Debug/CL_StateTreeDebugStateTask.h"

#include "StateTreeExecutionContext.h"

FCL_StateTreeDebugStateTask::FCL_StateTreeDebugStateTask()
{
	bShouldCallTick = false;

#if WITH_EDITORONLY_DATA
	bConsideredForCompletion = false;
	bCanEditConsideredForCompletion = false;
#endif
}

//~ Begin FStateTreeTaskCommonBase

EStateTreeRunStatus FCL_StateTreeDebugStateTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (InstanceData.bEnable && bPrintEnter)
	{
		
		GEngine->AddOnScreenDebugMessage(-1, 5.0, TextColor, FString::Printf(TEXT("Enter: %s"), *EnterText), true, FVector2D(FontScale, FontScale));
	}
	
	return EStateTreeRunStatus::Succeeded;
}

void FCL_StateTreeDebugStateTask::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (InstanceData.bEnable && bPrintExit)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0, TextColor, FString::Printf(TEXT("Exit: %s"), *ExitText), true, FVector2D(FontScale, FontScale));
	}
}

//~ End FStateTreeTaskCommonBase
