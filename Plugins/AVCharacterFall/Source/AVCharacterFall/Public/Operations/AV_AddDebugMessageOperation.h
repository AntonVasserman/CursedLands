// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AV_AddDebugMessageOperation.generated.h"

USTRUCT(BlueprintType)
struct AVCHARACTERFALL_API FAV_AddDebugMessageOperation
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "")
	uint8 bInLog:1 = false;

	UPROPERTY(EditDefaultsOnly, Category = "")
	uint8 bOnScreen:1 = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (EditCondition = "bInLog || bOnScreen", EditConditionHides))
	FString Message;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (EditCondition = "bOnScreen", EditConditionHides))
	FColor MessageColor = FColor::White;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (EditCondition = "bOnScreen", EditConditionHides))
	float Duration = 5.f;

	void Execute() const;
};
