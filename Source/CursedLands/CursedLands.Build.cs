// Copyright Anton Vasserman, All Rights Reserved.

using UnrealBuildTool;

public class CursedLands : ModuleRules
{
	public CursedLands(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"EnhancedInput",
				"GameplayCameras",
				"GameplayAbilities",
				"GameplayStateTreeModule",
				"StateTreeModule"
			});
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"AnimationWarpingRuntime",
				"AnimGraphRuntime",
				"AnimationLocomotionLibraryRuntime",
				"AVUtilities",
				"Chooser",
				"GameplayTags",
				"GameplayTasks",
				"MotionWarping"
			});
	}
}
