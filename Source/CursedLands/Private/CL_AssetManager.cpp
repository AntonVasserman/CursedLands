// Copyright Anton Vasserman, All Rights Reserved.


#include "CL_AssetManager.h"

#include "CL_LogChannels.h"

UCL_AssetManager& UCL_AssetManager::Get()
{
	check(GEngine);

	if (UCL_AssetManager* CLAssetManager = Cast<UCL_AssetManager>(GEngine->AssetManager))
	{
		return *CLAssetManager;
	}

	UE_LOG(LogCL, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini.  It must be set to UCL_AssetManager!"));

	// Fatal error above prevents this from being called.
	return *NewObject<UCL_AssetManager>();
}

//~ UAssetManager Begin

void UCL_AssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
}

//~ UAssetManager End
