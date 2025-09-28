// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CL_GameplayLoopDataAsset.generated.h"

UCLASS(BlueprintType, Const, Meta = (DisplayName = "Gameplay Loop Data"))
class CURSEDLANDS_API UCL_GameplayLoopDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 WaveCount = 1;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("GameplayLoopData", "Default");
	}
};
