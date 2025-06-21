// Copyright Anton Vasserman, All Rights Reserved.

#include "AV_DrawDebugStatics.h"
#include "DrawDebugHelpers.h"

void UAV_DrawDebugStatics::DrawDebugNamedDirectionalArrow(const UWorld* InWorld, const FVector& LineStart, const FVector& LineEnd, const FString& Name,
	const FColor& Color, const float MaxLength)
{
	FVector Direction = LineEnd - LineStart;
	const float CurrentLength = Direction.Size();
	FVector ClampedLineEnd = LineEnd;

	if (CurrentLength > MaxLength)
	{
		Direction.Normalize();
		ClampedLineEnd = LineStart + Direction * MaxLength;
	}
	
	DrawDebugDirectionalArrow(InWorld, LineStart, ClampedLineEnd, 5.f, Color, false, -1, 0, 2.5f);
	DrawDebugString(InWorld, ClampedLineEnd, Name, nullptr, Color, 0.f, false, 1.f);
}
