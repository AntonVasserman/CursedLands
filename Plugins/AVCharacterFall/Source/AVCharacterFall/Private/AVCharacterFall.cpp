// Copyright Epic Games, Inc. All Rights Reserved.

#include "AVCharacterFall.h"

#include "AV_CharacterFallLogChannels.h"

#define LOCTEXT_NAMESPACE "FAVCharacterFallModule"

//~ IModuleInterface Begin

void FAVCharacterFallModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UE_LOG(LogAV_CharacterFall, Display, TEXT("FAVCharacterFallModule: Startup"))
}

void FAVCharacterFallModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

//~ IModuleInterface End

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAVCharacterFallModule, AVCharacterFall)