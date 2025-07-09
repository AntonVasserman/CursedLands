// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Components/CL_ResourceComponent.h"
#include "Engine/DataAsset.h"
#include "CL_ResourceGameplayTagsData.generated.h"

UCLASS(BlueprintType)
class CURSEDLANDS_API UCL_ResourceGameplayTagsData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
	FCL_ResourceGameplayTags ResourceTags;
};
