// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

static TAutoConsoleVariable<bool> CVarCLShowDebugAnimationBlueprint(
	TEXT("CLCVar.Debug.AnimationBlueprint"),
	false,
	TEXT("Determines if Animation Blueprint Data should be shown on screen, used for debugging."),
	ECVF_Default);