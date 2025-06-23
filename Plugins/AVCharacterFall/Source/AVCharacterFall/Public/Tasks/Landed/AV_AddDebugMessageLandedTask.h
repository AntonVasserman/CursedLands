// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AV_LandedTaskBase.h"
#include "Operations/AV_AddDebugMessageOperation.h"
#include "AV_AddDebugMessageLandedTask.generated.h"

UCLASS(Category = "Character Fall|Tasks|Landed", Meta = (DisplayName = "Add Debug Message"))
class AVCHARACTERFALL_API UAV_AddDebugMessageLandedTask : public UAV_LandedTaskBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "")
	FAV_AddDebugMessageOperation Operation;

	//~ UAV_LandedTaskBase Begin
public:
	virtual void ExecuteTaskInternal_Implementation(const FAV_LandedTaskContext& TaskContext) const override;
	//~ UAV_LandedTaskBase End
};
