// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_LandedTaskBase.h"
#include "Engine/DataAsset.h"
#include "CL_PrintStringLandedTask.generated.h"

UCLASS(EditInlineNew, Meta = (DisplayName = "Print String"))
class CURSEDLANDS_API UCL_PrintStringLandedTask : public UCL_LandedTaskBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "")
	FString StringToPrint;

	UPROPERTY(EditDefaultsOnly, Category = "")
	FColor StringColor;

	UPROPERTY(EditDefaultsOnly, Category = "")
	float PrintDuration;

	//~ UCLLandedTaskBase Begin
public:	
	virtual void ExecuteTask_Implementation(const FCL_LandedTaskContext& TaskContext) const override;
	//~ UCLLandedTaskBase End
};
