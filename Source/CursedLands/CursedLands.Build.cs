// Copyright Anton Vasserman, All Rights Reserved.

using UnrealBuildTool;

public class CursedLands : ModuleRules
{
	public CursedLands(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "GameplayCameras", "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
