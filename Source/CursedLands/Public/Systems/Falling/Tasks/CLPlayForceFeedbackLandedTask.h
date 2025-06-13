// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLLandedTaskBase.h"
#include "CLPlayForceFeedbackLandedTask.generated.h"

UCLASS(Blueprintable, EditInlineNew)
class CURSEDLANDS_API UCLPlayForceFeedbackLandedTask : public UCLLandedTaskBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UForceFeedbackEffect> ForceFeedbackEffect;
	
public:
	virtual void ExecuteTask(const FCLLandedTaskContext& TaskContext) const override;
};
