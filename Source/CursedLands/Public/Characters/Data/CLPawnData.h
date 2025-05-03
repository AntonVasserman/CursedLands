// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CLPawnData.generated.h"

class UCLAbilitySet;

UCLASS(BlueprintType, Const)
class CURSEDLANDS_API UCLPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|GameplayAbilitySystem")
	TArray<TObjectPtr<UCLAbilitySet>> AbilitySets;
};
