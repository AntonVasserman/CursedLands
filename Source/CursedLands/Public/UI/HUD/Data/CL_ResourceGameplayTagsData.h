// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Components/CL_ResourceComponent.h"
#include "Engine/DataAsset.h"
#include "CL_ResourceGameplayTagsData.generated.h"

UCLASS()
class CURSEDLANDS_API UCL_ResourceGameplayTagsData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	// TODO ???
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource")
	// FText ResourceDisplayName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
	FCL_ResourceGameplayTags ResourceTags;
};
