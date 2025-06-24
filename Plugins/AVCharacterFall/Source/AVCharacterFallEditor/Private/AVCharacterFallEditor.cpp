// Copyright Epic Games, Inc. All Rights Reserved.

#include "AVCharacterFallEditor.h"

#include "PropertyEditorModule.h"
#include "Components/AV_CharacterFallComponent.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

#define LOCTEXT_NAMESPACE "FAVCharacterFallModule"

void FAVCharacterFallEditorModule::SetupCustomIcons()
{
	static TSharedPtr<FSlateStyleSet> StyleSet = nullptr;
	
	if (!StyleSet.IsValid())
	{
		// Setup StyleSet
		StyleSet = MakeShareable(new FSlateStyleSet("AVCharacterFallStyle"));
		if (const TSharedPtr<IPlugin> CurrentPlugin = IPluginManager::Get().FindPlugin(TEXT("AVCharacterFall"));
			CurrentPlugin.IsValid())
		{
			const FString ContentDir = CurrentPlugin->GetContentDir();
			StyleSet->SetContentRoot(ContentDir / TEXT("Editor/Slate"));
		}

		// Setup-ups
		SetupCharacterFallComponentIcon(StyleSet.Get());
		
		FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
	}
}

void FAVCharacterFallEditorModule::SetupCharacterFallComponentIcon(FSlateStyleSet* StyleSet)
{
	// Old example code to setup the Icon to the CharacterMovementComponent Icon:
	/*
	if (const FSlateBrush* EngineCMCBrush = FAppStyle::Get().GetBrush(TEXT("ClassIcon.CharacterMovementComponent"));
			EngineCMCBrush != nullptr)
	{
		StyleSet->Set(
			*FString::Printf(TEXT("ClassIcon.%s"), *UAV_CharacterFallComponent::StaticClass()->GetName()),
			new FSlateBrush(*EngineCMCBrush));   // Make a copy so it isn't a const.

	}
	*/
	
	const FString CFCIconPropName = FString::Printf(TEXT("ClassIcon.%s"), *UAV_CharacterFallComponent::StaticClass()->GetName());
	const FString CFCIconPath = FString::Printf(TEXT("/Icons/%sClassIcon"), *UAV_CharacterFallComponent::StaticClass()->GetName());
	FSlateImageBrush* CFCIconBrush = new FSlateImageBrush(StyleSet->RootToContentDir(CFCIconPath, TEXT(".png")), FVector2D(20.f, 20.f));

	StyleSet->Set(*CFCIconPropName, CFCIconBrush);
}

//~ IModuleInterface Begin

void FAVCharacterFallEditorModule::StartupModule()
{
	SetupCustomIcons();
}

void FAVCharacterFallEditorModule::ShutdownModule()
{
}

//~ IModuleInterface End

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FAVCharacterFallEditorModule, AVCharacterFallEditor)
