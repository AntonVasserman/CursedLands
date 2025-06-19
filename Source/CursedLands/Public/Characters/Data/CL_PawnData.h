// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CL_PawnData.generated.h"

class UCL_AbilitySet;

UCLASS(BlueprintType, Const)
class CURSEDLANDS_API UCL_PawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|GameplayAbilitySystem")
	TArray<TObjectPtr<UCL_AbilitySet>> AbilitySets;
};
