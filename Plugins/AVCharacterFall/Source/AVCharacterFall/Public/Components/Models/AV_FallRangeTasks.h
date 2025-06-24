// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AV_FallRange.h"
#include "UObject/Object.h"
#include "AV_FallRangeTasks.generated.h"

class ACharacter;
class UAV_CharacterFallComponent;
class UAV_LandedTaskBase;
class UAV_FallingTaskBase;

UENUM()
enum class EAV_FallRangeState : uint8
{
	Outside		UMETA(DisplayName = "Outside", ToolTip = "Outside of the fall range"),
	Inside		UMETA(DisplayName = "Inside", ToolTip = "Inside of the fall range"),
};

USTRUCT()
struct AVCHARACTERFALL_API FAV_FallRangeContext
{
	GENERATED_BODY()

	UPROPERTY(Category = "")
	TObjectPtr<ACharacter> Character = nullptr;

	UPROPERTY(Category = "")
	TObjectPtr<UAV_CharacterFallComponent> CharacterFallComponent = nullptr;
	
	UPROPERTY(Category = "")
	float FallHeight = 0.f;

	UPROPERTY(Category = "")
	TObjectPtr<AActor> HitActor = nullptr;
};

USTRUCT()
struct AVCHARACTERFALL_API FAV_FallRangeTasks
{
	GENERATED_BODY()

	EAV_FallRangeState State = EAV_FallRangeState::Outside;
	
	UPROPERTY(EditDefaultsOnly, Category = "")
	FAV_FallRange FallRange;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (BaseClass = UAV_LandedTaskBase), Instanced)
	TArray<TObjectPtr<UAV_LandedTaskBase>> LandedTasks;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (BaseClass = UAV_FallingTaskBase), Instanced)
	TArray<TObjectPtr<UAV_FallingTaskBase>> FallingEnterTasks;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (BaseClass = UAV_FallingTaskBase), Instanced)
	TArray<TObjectPtr<UAV_FallingTaskBase>> FallingExitTasks;

	EAV_FallRangeState TestRange(const FAV_FallRangeContext& ConditionContext);

private:
	void ExecuteLandedTasks(const FAV_FallRangeContext& ConditionContext);
	void ExecuteFallingEnterTasks(const FAV_FallRangeContext& ConditionContext);
	void ExecuteFallingExitTasks(const FAV_FallRangeContext& ConditionContext);
};
