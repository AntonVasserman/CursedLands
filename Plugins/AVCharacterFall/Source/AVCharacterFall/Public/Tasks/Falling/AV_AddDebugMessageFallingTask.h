// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AV_FallingTaskBase.h"
#include "Operations/AV_AddDebugMessageOperation.h"
#include "AV_AddDebugMessageFallingTask.generated.h"

UCLASS(Category = "Character Fall|Tasks|Falling", Meta = (DisplayName = "Add Debug Message"))
class AVCHARACTERFALL_API UAV_AddDebugMessageFallingTask : public UAV_FallingTaskBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "")
	FAV_AddDebugMessageOperation Operation;

	//~ UAV_FallingTaskBase Begin
public:
	virtual void ExecuteTask_Implementation(const FAV_FallingTaskContext& TaskContext) const override;
	//~ UAV_FallingTaskBase End
};
