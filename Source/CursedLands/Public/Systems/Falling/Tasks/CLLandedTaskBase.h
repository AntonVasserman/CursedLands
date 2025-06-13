// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLLandedTaskBase.generated.h"

USTRUCT()
struct CURSEDLANDS_API FCLLandedTaskContext
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;
};

UCLASS(Blueprintable, EditInlineNew)
class CURSEDLANDS_API UCLLandedTaskBase : public UObject
{
	GENERATED_BODY()
	
public:	
	virtual void ExecuteTask(const FCLLandedTaskContext& TaskContext) const {}
};
