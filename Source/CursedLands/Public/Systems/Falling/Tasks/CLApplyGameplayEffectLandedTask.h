// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLLandedTaskBase.h"
#include "GameplayEffect.h"
#include "CLApplyGameplayEffectLandedTask.generated.h"

class UAbilitySystemComponent;

UENUM()
enum class ECLGameplayEffectLevelType
{
	Absolute				UMETA(DisplayName = "Absolute", Description = "Gameplay Effect Level is a set value" ),
	RelativeToFallHeight	UMETA(DisplayName = "Relative to Fall Height", Description = "Gameplay Effect Level is based on the Fall Height. Mapped from [MinFallHeight,MaxFallHeight] to [MinGameplayEffectLevel,MaxGameplayEffectLevel].")
};

UCLASS(EditInlineNew, Meta = (DisplayName = "Apply Gameplay Effect"))
class CURSEDLANDS_API UCLApplyGameplayEffectLandedTask : public UCLLandedTaskBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "")
	TSubclassOf<UGameplayEffect> GameplayEffectClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (EditCondition = "GameplayEffectClass != nullptr", EditConditionHides))
	ECLGameplayEffectLevelType GameplayEffectLevelType = ECLGameplayEffectLevelType::Absolute;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (EditCondition = "GameplayEffectClass != nullptr && GameplayEffectLevelType == ECLGameplayEffectLevelType::Absolute", EditConditionHides))
	float GameplayEffectLevel = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (EditCondition = "GameplayEffectClass != nullptr && GameplayEffectLevelType == ECLGameplayEffectLevelType::RelativeToFallHeight", EditConditionHides))
	float MinFallHeight = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (EditCondition = "GameplayEffectClass != nullptr && GameplayEffectLevelType == ECLGameplayEffectLevelType::RelativeToFallHeight", EditConditionHides))
	float MaxFallHeight = 100.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (EditCondition = "GameplayEffectClass != nullptr && GameplayEffectLevelType == ECLGameplayEffectLevelType::RelativeToFallHeight", EditConditionHides))
	float MinGameplayEffectLevel = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (EditCondition = "GameplayEffectClass != nullptr && GameplayEffectLevelType == ECLGameplayEffectLevelType::RelativeToFallHeight", EditConditionHides))
	float MaxGameplayEffectLevel = 1.f;

private:
	float GetLevelToApply(const FCLLandedTaskContext& TaskContext) const;

	//~ UCLLandedTaskBase Begin
public:
	virtual void ExecuteTask_Implementation(const FCLLandedTaskContext& TaskContext) const override;
	//~ UCLLandedTaskBase End
};
