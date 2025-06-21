// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AV_DrawDebugStatics.generated.h"

UCLASS()
class AVUTILITIES_API UAV_DrawDebugStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "AVUtilities|DrawDebug")
	static void DrawDebugNamedDirectionalArrow(const UWorld* InWorld, const FVector& LineStart, const FVector& LineEnd, const FString& Name,
		const FColor& Color, const float MaxLength = 250.f);
};
