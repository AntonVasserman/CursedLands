// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "CL_AssetManager.generated.h"

UCLASS()
class CURSEDLANDS_API UCL_AssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	static UCL_AssetManager& Get();

	//~ UAssetManager Begin
protected:
	virtual void StartInitialLoading() override;
	//~ UAssetManager End
};
