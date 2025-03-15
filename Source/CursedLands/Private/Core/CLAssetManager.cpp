// Copyright Anton Vasserman, All Rights Reserved.


#include "Core/CLAssetManager.h"

#include "Core/CLGameplayTags.h"

UCLAssetManager& UCLAssetManager::Get()
{
	check(GEngine);

	UCLAssetManager* CLAssetManager = Cast<UCLAssetManager>(GEngine->AssetManager);
	return *CLAssetManager;
}

//~ UAssetManager Begin

void UCLAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FCLGameplayTags::InitializeNativeGameplayTags();
}

//~ UAssetManager End
