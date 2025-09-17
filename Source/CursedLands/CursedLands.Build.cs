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
				"AIModule",
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"EnhancedInput",
				"GameplayCameras",
				"GameplayAbilities",
				"GameplayTags",
				"GameplayTasks",
				"GameplayStateTreeModule",
				"ModelViewViewModel",
				"NavigationSystem",
				"PoseSearch",
				"PropertyBindingUtils",
				"Slate",
				"SlateCore",
				"StateTreeModule",
				"UMG",
			});
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"AnimationWarpingRuntime",
				"AnimGraphRuntime",
				"AnimationLocomotionLibraryRuntime",
				"AudioModulation",
				"AVUtilities",
				"Chooser",
				"CommonInput",
				"CommonLoadingScreen",
				"CommonUI",
				"DeveloperSettings",
				"GameSettings",
				"MotionWarping"
			});
	}
}
