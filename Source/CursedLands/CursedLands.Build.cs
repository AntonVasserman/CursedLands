// Copyright Anton Vasserman, All Rights Reserved.

using UnrealBuildTool;

public class CursedLands : ModuleRules
{
	public CursedLands(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayCameras", "GameplayAbilities" });
		PrivateDependencyModuleNames.AddRange(new string[] { "AnimGraphRuntime", "AnimationLocomotionLibraryRuntime", "AVUtilities", "GameplayTags", "GameplayTasks" });
	}
}
