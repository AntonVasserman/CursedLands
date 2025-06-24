// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FAVCharacterFallModule : public IModuleInterface
{
	//~ IModuleInterface Begin
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	//~ IModuleInterface End
};
