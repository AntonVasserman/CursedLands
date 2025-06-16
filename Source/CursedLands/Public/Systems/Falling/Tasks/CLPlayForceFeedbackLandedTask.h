// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLLandedTaskBase.h"
#include "CLPlayForceFeedbackLandedTask.generated.h"

UCLASS(EditInlineNew, Meta = (DisplayName = "Play Force Feedback"))
class CURSEDLANDS_API UCLPlayForceFeedbackLandedTask : public UCLLandedTaskBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "")
	TObjectPtr<UForceFeedbackEffect> ForceFeedbackEffect;

	//~ UCLLandedTaskBase Begin
public:
	virtual void ExecuteTask_Implementation(const FCLLandedTaskContext& TaskContext) const override;
	//~ UCLLandedTaskBase End
};
