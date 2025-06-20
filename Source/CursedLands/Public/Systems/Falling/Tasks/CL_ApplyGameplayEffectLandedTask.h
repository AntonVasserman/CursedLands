// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_LandedTaskBase.h"
#include "GameplayEffect.h"
#include "CL_ApplyGameplayEffectLandedTask.generated.h"

class UAbilitySystemComponent;

UENUM()
enum class ECL_GameplayEffectLevelType
{
	Absolute				UMETA(DisplayName = "Absolute", Description = "Gameplay Effect Level is a set value" ),
	RelativeToFallHeight	UMETA(DisplayName = "Relative to Fall Height", Description = "Gameplay Effect Level is based on the Fall Height. Mapped from [MinFallHeight,MaxFallHeight] to [MinGameplayEffectLevel,MaxGameplayEffectLevel].")
};

UCLASS(EditInlineNew, Meta = (DisplayName = "Apply Gameplay Effect"))
class CURSEDLANDS_API UCL_ApplyGameplayEffectLandedTask : public UCL_LandedTaskBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "")
	TSubclassOf<UGameplayEffect> GameplayEffectClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (EditCondition = "GameplayEffectClass != nullptr", EditConditionHides))
	ECL_GameplayEffectLevelType GameplayEffectLevelType = ECL_GameplayEffectLevelType::Absolute;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (EditCondition = "GameplayEffectClass != nullptr && GameplayEffectLevelType == ECL_GameplayEffectLevelType::Absolute", EditConditionHides))
	float GameplayEffectLevel = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (EditCondition = "GameplayEffectClass != nullptr && GameplayEffectLevelType == ECL_GameplayEffectLevelType::RelativeToFallHeight", EditConditionHides))
	float MinFallHeight = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (EditCondition = "GameplayEffectClass != nullptr && GameplayEffectLevelType == ECL_GameplayEffectLevelType::RelativeToFallHeight", EditConditionHides))
	float MaxFallHeight = 100.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (EditCondition = "GameplayEffectClass != nullptr && GameplayEffectLevelType == ECL_GameplayEffectLevelType::RelativeToFallHeight", EditConditionHides))
	float MinGameplayEffectLevel = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (EditCondition = "GameplayEffectClass != nullptr && GameplayEffectLevelType == ECL_GameplayEffectLevelType::RelativeToFallHeight", EditConditionHides))
	float MaxGameplayEffectLevel = 1.f;

private:
	float GetLevelToApply(const FCL_LandedTaskContext& TaskContext) const;

	//~ UCLLandedTaskBase Begin
public:
	virtual void ExecuteTaskInternal_Implementation(const FCL_LandedTaskContext& TaskContext) const override;
	//~ UCLLandedTaskBase End
};
