// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Tasks/Falling/AV_FallingTaskBase.h"
#include "CL_GASOperationFallingTask.generated.h"

class UCL_GASOperationBase;

UCLASS(Meta = (DisplayName = "GAS Operation"))
class CURSEDLANDS_API UCL_GASOperationFallingTask : public UAV_FallingTaskBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Instanced, Category = "")
	TObjectPtr<UCL_GASOperationBase> GASOperation = nullptr;

	//~ UAV_FallingTaskBase Begin
public:
	virtual void ExecuteTask_Implementation(const FAV_FallingTaskContext& TaskContext) const override;
	//~ UAV_FallingTaskBase End
};
