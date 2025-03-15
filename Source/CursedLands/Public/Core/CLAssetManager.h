// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "CLAssetManager.generated.h"

UCLASS()
class CURSEDLANDS_API UCLAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	static UCLAssetManager& Get();

	//~ UAssetManager Begin

protected:
	virtual void StartInitialLoading() override;
	
	//~ UAssetManager End
};
