// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLLandedTaskBase.h"
#include "Engine/DataAsset.h"
#include "CLPrintStringLandedTask.generated.h"

UCLASS(EditInlineNew, Meta = (DisplayName = "Print String"))
class CURSEDLANDS_API UCLPrintStringLandedTask : public UCLLandedTaskBase
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
	virtual void ExecuteTask_Implementation(const FCLLandedTaskContext& TaskContext) const override;
	//~ UCLLandedTaskBase End
};
