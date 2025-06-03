// Copyright Anton Vasserman, All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class CursedLandsEditorTarget : TargetRules
{
	public CursedLandsEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("CursedLands");
	}
}
