// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FAVUtilitiesModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
