// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CL_ScoreSubsystem.generated.h"

UCLASS()
class CURSEDLANDS_API UCL_ScoreSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetScore() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(const int32 InScoreToAdd);

private:
	uint32 CurrentScore = 0;
};
