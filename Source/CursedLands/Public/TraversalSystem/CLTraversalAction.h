// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECLTraversalAction : uint8
{
	None	UMETA(DisplayName = "None", Description = "No action"),
	Hurdle	UMETA(DisplayName = "Hurdle", Description = "Traverse over a thin object and end on the ground at a similar level (Low fence)"),
	Vault	UMETA(DisplayName = "Vault", Description = "Traverse over a thin object and end in a falling state (Tall fence, or elevated obstacle with no floor on the other side)"),
	Mantle	UMETA(DisplayName = "Mantle", Description = "Traverse up and onto an object without passing over it"),
	Climb	UMETA(DisplayName = "Climb", Description = "Traverse up and onto a higher object without passing over it"),
	Slide 	UMETA(DisplayName = "Slide", Description = "Traverse under an object"),
};
