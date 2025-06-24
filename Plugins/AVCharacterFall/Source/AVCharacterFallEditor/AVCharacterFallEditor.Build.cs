// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AVCharacterFallEditor : ModuleRules
{
	public AVCharacterFallEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			});

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"AVCharacterFall",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
			});
		
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(new string[] { "Projects" });
		}
	}
}