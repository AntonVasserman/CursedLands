// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FSlateStyleSet;

class FAVCharacterFallEditorModule : public IModuleInterface
{
	void SetupCustomIcons();
	void SetupCharacterFallComponentIcon(FSlateStyleSet* StyleSet);
	
	//~ IModuleInterface Begin
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	//~ IModuleInterface End
};
