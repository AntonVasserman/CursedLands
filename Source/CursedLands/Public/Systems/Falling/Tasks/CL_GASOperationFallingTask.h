// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_FallingTaskBase.h"
#include "UObject/Object.h"
#include "CL_GASOperationFallingTask.generated.h"

class UCL_CharacterFallingComponent;
class UCL_GASOperationBase;

UCLASS(EditInlineNew, Meta = (DisplayName = "GAS Operation"))
class CURSEDLANDS_API UCL_GASOperationFallingTask : public UCL_FallingTaskBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Instanced, Category = "")
	TObjectPtr<UCL_GASOperationBase> GASOperation = nullptr;

	//~ UCL_FallingTaskBase Begin
public:
	virtual void ExecuteTask_Implementation(const FCL_FallingTaskContext& TaskContext) const override;
	//~ UCL_FallingTaskBase End
};
