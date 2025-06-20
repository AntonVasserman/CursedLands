// Copyright Anton Vasserman, All Rights Reserved.

#include "Systems/Falling/Tasks/CL_PrintStringLandedTask.h"

//~ UCLLandedTaskBase Begin

void UCL_PrintStringLandedTask::ExecuteTaskInternal_Implementation(const FCL_LandedTaskContext& TaskContext) const
{
	GEngine->AddOnScreenDebugMessage(-1, PrintDuration, StringColor, *StringToPrint);
}

//~ UCLLandedTaskBase End