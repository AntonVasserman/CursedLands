// Copyright Anton Vasserman, All Rights Reserved.

#include "Systems/Falling/Tasks/CL_PrintStringFallingTask.h"

//~ UCL_FallingTaskBase Begin

void UCL_PrintStringFallingTask::ExecuteTask_Implementation(const FCL_FallingTaskContext& TaskContext) const
{
	GEngine->AddOnScreenDebugMessage(-1, PrintDuration, StringColor, *StringToPrint);
}

//~ UCL_FallingTaskBase End
