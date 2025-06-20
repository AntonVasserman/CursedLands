// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Systems/Falling/Tasks/CL_FallingTaskBase.h"
#include "CL_PrintStringFallingTask.generated.h"

UCLASS(EditInlineNew, Meta = (DisplayName = "Print String"))
class CURSEDLANDS_API UCL_PrintStringFallingTask : public UCL_FallingTaskBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "")
	FString StringToPrint;

	UPROPERTY(EditDefaultsOnly, Category = "")
	FColor StringColor;

	UPROPERTY(EditDefaultsOnly, Category = "")
	float PrintDuration;

	//~ UCL_FallingTaskBase Begin
public:
	virtual void ExecuteTask_Implementation(const FCL_FallingTaskContext& TaskContext) const override;
	//~ UCL_FallingTaskBase End
};
