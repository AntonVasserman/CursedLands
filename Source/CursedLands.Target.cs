// Copyright Anton Vasserman, All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class CursedLandsTarget : TargetRules
{
	public CursedLandsTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		ExtraModuleNames.Add("CursedLands");
	}
}
