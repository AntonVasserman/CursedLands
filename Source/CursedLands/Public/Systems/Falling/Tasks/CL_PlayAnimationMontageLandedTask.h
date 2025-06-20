// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_LandedTaskBase.h"
#include "CL_PlayAnimationMontageLandedTask.generated.h"

UCLASS(EditInlineNew, Meta = (DisplayName = "Play Animation Montage"))
class CURSEDLANDS_API UCL_PlayAnimationMontageLandedTask : public UCL_LandedTaskBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "")
	TObjectPtr<UAnimMontage> AnimMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (EditCondition = "AnimMontage != nullptr", EditConditionHides))
	uint8 bJumpToSection:1 = false;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (EditCondition = "AnimMontage != nullptr && bJumpToSection == true", EditConditionHides))
	FName SectionName;

	//~ UCLLandedTaskBase Begin
public:
	virtual void ExecuteTaskInternal_Implementation(const FCL_LandedTaskContext& TaskContext) const override;
	//~ UCLLandedTaskBase End
};
