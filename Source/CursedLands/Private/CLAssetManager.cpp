// Copyright Anton Vasserman, All Rights Reserved.


#include "CLAssetManager.h"

#include "CLGameplayTags.h"

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
