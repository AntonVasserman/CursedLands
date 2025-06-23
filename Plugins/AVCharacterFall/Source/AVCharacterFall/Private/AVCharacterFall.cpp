// Copyright Epic Games, Inc. All Rights Reserved.

#include "AVCharacterFall.h"
#include "Components/AV_CharacterFallComponent.h"
#include "Styling/AppStyle.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateTypes.h"

#define LOCTEXT_NAMESPACE "FAVCharacterFallModule"

void FAVCharacterFallModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Set custom Class Icons
	static TSharedPtr<FSlateStyleSet> StyleSet = nullptr;
	if (!StyleSet.IsValid())
	{
		StyleSet = MakeShareable(new FSlateStyleSet("AVCharacterFallStyle"));

		// Update the CharacterFallComponent Icon
		// Search for the CharacterMovementIcon using FAppStyle
		if (const FSlateBrush* EngineCMCBrush = FAppStyle::Get().GetBrush(TEXT("ClassIcon.CharacterMovementComponent"));
			EngineCMCBrush != nullptr)
		{
			StyleSet->Set(
				*FString::Printf(TEXT("ClassIcon.%s"), *UAV_CharacterFallComponent::StaticClass()->GetName()),
				new FSlateBrush(*EngineCMCBrush));   // Make a copy so it isn't a const.

		}

		// Register the style set so it is actually used
		FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
	}
}

void FAVCharacterFallModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAVCharacterFallModule, AVCharacterFall)