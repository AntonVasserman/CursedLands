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
	RelativeToFallHeight	UMETA(DisplayName = "Relative to Fall Height", Description = "Gameplay Effect Level is based on the Fall Height mapped from the Landed Condition range to the min/max values assigned. In case of a non-range condition will always result in max value assigned.")
};

UCLASS(Blueprintable, EditInlineNew)
class CURSEDLANDS_API UCLApplyGameplayEffectLandedTask : public UCLLandedTaskBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffectClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true", EditCondition = "GameplayEffectClass != nullptr", EditConditionHides))
	ECLGameplayEffectLevelType GameplayEffectLevelType = ECLGameplayEffectLevelType::Absolute;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true", EditCondition = "GameplayEffectClass != nullptr && GameplayEffectLevelType == ECLGameplayEffectLevelType::Absolute", EditConditionHides))
	float GameplayEffectLevel = 1.f;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true", EditCondition = "GameplayEffectClass != nullptr && GameplayEffectLevelType == ECLGameplayEffectLevelType::RelativeToFallHeight", EditConditionHides))
	float MinGameplayEffectLevel = 0.f;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true", EditCondition = "GameplayEffectClass != nullptr && GameplayEffectLevelType == ECLGameplayEffectLevelType::RelativeToFallHeight", EditConditionHides))
	float MaxGameplayEffectLevel = 1.f;

private:
	TOptional<UAbilitySystemComponent*> GetAbilitySystemComponent(const ACharacter* Character) const;
	float GetLevelToApply(const FCLLandedTaskContext& TaskContext) const;

	//~ UCLLandedTaskBase Begin
public:
	virtual void ExecuteTask(const FCLLandedTaskContext& TaskContext) const override;
	//~ UCLLandedTaskBase End
};
