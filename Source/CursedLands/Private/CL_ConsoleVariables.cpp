// Copyright Anton Vasserman, All Rights Reserved.

#include "CL_ConsoleVariables.h"

TAutoConsoleVariable<bool> CVarCLShowDebugAnimationBlueprint(
	TEXT("CLCVar.Debug.AnimationBlueprint"),
	false,
	TEXT("Determines if Animation Blueprint Data should be shown on screen, used for debugging."),
	ECVF_Default);
