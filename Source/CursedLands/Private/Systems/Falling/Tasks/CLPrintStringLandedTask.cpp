// Copyright Anton Vasserman, All Rights Reserved.


#include "Systems/Falling/Tasks/CLPrintStringLandedTask.h"

void UCLPrintStringLandedTask::ExecuteTask(const FCLLandedTaskContext& TaskContext) const
{
	UCLLandedTaskBase::ExecuteTask(TaskContext);
	
	GEngine->AddOnScreenDebugMessage(-1, PrintDuration, StringColor, *StringToPrint);
}
