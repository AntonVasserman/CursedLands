// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CLPrintStringLandedTask.generated.h"

UCLASS()
class CURSEDLANDS_API UCLPrintStringLandedTask : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	FString StringToPrint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	FColor StringColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	float PrintDuration;
	
	void ExecuteTask() const;
};
