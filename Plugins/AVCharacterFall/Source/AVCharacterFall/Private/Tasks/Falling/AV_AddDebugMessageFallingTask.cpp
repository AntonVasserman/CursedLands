// Copyright Anton Vasserman, All Rights Reserved.

#include "Tasks/Falling/AV_AddDebugMessageFallingTask.h"

//~ UAV_FallingTaskBase Begin

void UAV_AddDebugMessageFallingTask::ExecuteTask_Implementation(const FAV_FallingTaskContext& TaskContext) const
{
	Operation.Execute();
}

//~ UAV_FallingTaskBase End
