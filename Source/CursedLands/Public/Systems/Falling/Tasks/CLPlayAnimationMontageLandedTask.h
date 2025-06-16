// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLLandedTaskBase.h"
#include "CLPlayAnimationMontageLandedTask.generated.h"

UCLASS(EditInlineNew, Meta = (DisplayName = "Play Animation Montage"))
class CURSEDLANDS_API UCLPlayAnimationMontageLandedTask : public UCLLandedTaskBase
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
	virtual void ExecuteTask_Implementation(const FCLLandedTaskContext& TaskContext) const override;
	//~ UCLLandedTaskBase End
};
