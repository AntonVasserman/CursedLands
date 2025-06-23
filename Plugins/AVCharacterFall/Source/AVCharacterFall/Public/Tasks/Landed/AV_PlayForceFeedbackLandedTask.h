// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AV_LandedTaskBase.h"
#include "AV_PlayForceFeedbackLandedTask.generated.h"

class UForceFeedbackEffect;

UCLASS(Category = "Character Fall|Tasks|Landed", Meta = (DisplayName = "Play Force Feedback"))
class AVCHARACTERFALL_API UAV_PlayForceFeedbackLandedTask : public UAV_LandedTaskBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "")
	TObjectPtr<UForceFeedbackEffect> ForceFeedbackEffect = nullptr;

	//~ UAV_LandedTaskBase Begin
protected:
	virtual void ExecuteTaskInternal_Implementation(const FAV_LandedTaskContext& TaskContext) const override;
	//~ UAV_LandedTaskBase End
};
