// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLLandedTaskBase.h"
#include "Engine/DataAsset.h"
#include "CLPrintStringLandedTask.generated.h"

UCLASS(Blueprintable, EditInlineNew)
class CURSEDLANDS_API UCLPrintStringLandedTask : public UCLLandedTaskBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	FString StringToPrint;

	UPROPERTY(EditDefaultsOnly)
	FColor StringColor;

	UPROPERTY(EditDefaultsOnly)
	float PrintDuration;
	
public:	
	virtual void ExecuteTask(const FCLLandedTaskContext& TaskContext) const override;
};
