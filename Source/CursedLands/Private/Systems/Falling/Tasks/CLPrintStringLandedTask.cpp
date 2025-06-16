// Copyright Anton Vasserman, All Rights Reserved.

#include "Systems/Falling/Tasks/CLPrintStringLandedTask.h"

//~ UCLLandedTaskBase Begin

void UCLPrintStringLandedTask::ExecuteTask_Implementation(const FCLLandedTaskContext& TaskContext) const
{
	GEngine->AddOnScreenDebugMessage(-1, PrintDuration, StringColor, *StringToPrint);
}

//~ UCLLandedTaskBase End