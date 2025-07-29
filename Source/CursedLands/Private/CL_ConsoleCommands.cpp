// Copyright Anton Vasserman, All Rights Reserved.

#include "CL_ConsoleCommands.h"
#include "CL_ConsoleVariables.h"
#include "HAL/IConsoleManager.h"

static FAutoConsoleCommand CCmdShowDebugAnimationBlueprint(
TEXT("CLCCmd.ShowDebug.AnimationBlueprint"),
	TEXT("Toggles Animation Blueprint debug display (with Motion Matching Pose Search Trajectory)"),
	FConsoleCommandDelegate::CreateStatic(&CL_ConsoleCommands::CLShowDebug_AnimationBlueprint),
	ECVF_Default);

void CL_ConsoleCommands::CLShowDebug_AnimationBlueprint()
{
	const bool bNewValue = !CVarCLShowDebugAnimationBlueprint->GetBool();

	CVarCLShowDebugAnimationBlueprint->Set(bNewValue);
	const FString DebugDrawTrajectoryCommand = FString::Printf(TEXT("a.AnimNode.PoseHistory.DebugDrawTrajectory %d"), bNewValue ? 1 : 0);
	IConsoleManager::Get().ProcessUserConsoleInput(*DebugDrawTrajectoryCommand, *GLog, nullptr);
}
