// Copyright Anton Vasserman, All Rights Reserved.

#include "Tasks/Landed/AV_AddDebugMessageLandedTask.h"

//~ UAV_LandedTaskBase Begin

void UAV_AddDebugMessageLandedTask::ExecuteTaskInternal_Implementation(const FAV_LandedTaskContext& TaskContext) const
{
	Operation.Execute();
}

//~ UAV_LandedTaskBase End
