// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AV_LandedTaskBase.h"
#include "AV_PlayAnimationMontageLandedTask.generated.h"

class UAnimMontage;

UCLASS(Category = "Character Fall|Tasks|Landed", Meta = (DisplayName = "Play Animation Montage"))
class AVCHARACTERFALL_API UAV_PlayAnimationMontageLandedTask : public UAV_LandedTaskBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "")
	TObjectPtr<UAnimMontage> AnimMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (EditCondition = "AnimMontage != nullptr", EditConditionHides))
	uint8 bJumpToSection:1 = false;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (EditCondition = "AnimMontage != nullptr && bJumpToSection == true", EditConditionHides))
	FName SectionName;
	
	//~ UAV_LandedTaskBase Begin
protected:
	virtual void ExecuteTaskInternal_Implementation(const FAV_LandedTaskContext& TaskContext) const override;
	//~ UAV_LandedTaskBase End
};
