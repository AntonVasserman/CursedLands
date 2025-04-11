// Copyright Anton Vasserman, All Rights Reserved.


#include "CLAssetManager.h"

#include "CLGameplayTags.h"
#include "CLLogChannels.h"

UCLAssetManager& UCLAssetManager::Get()
{
	check(GEngine);

	if (UCLAssetManager* CLAssetManager = Cast<UCLAssetManager>(GEngine->AssetManager))
	{
		return *CLAssetManager;
	}

	UE_LOG(LogCL, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini.  It must be set to UCLAssetManager!"));

	// Fatal error above prevents this from being called.
	return *NewObject<UCLAssetManager>();
}

//~ UAssetManager Begin

void UCLAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
}

//~ UAssetManager End
