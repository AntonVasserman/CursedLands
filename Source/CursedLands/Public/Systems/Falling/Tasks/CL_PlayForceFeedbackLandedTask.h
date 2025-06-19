// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_LandedTaskBase.h"
#include "CL_PlayForceFeedbackLandedTask.generated.h"

UCLASS(EditInlineNew, Meta = (DisplayName = "Play Force Feedback"))
class CURSEDLANDS_API UCL_PlayForceFeedbackLandedTask : public UCL_LandedTaskBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "")
	TObjectPtr<UForceFeedbackEffect> ForceFeedbackEffect;

	//~ UCLLandedTaskBase Begin
public:
	virtual void ExecuteTask_Implementation(const FCL_LandedTaskContext& TaskContext) const override;
	//~ UCLLandedTaskBase End
};
