// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "StateTreeTaskBase.h"
#include "UObject/Object.h"
#include "CL_ApplyGameplayEffectStateTreeTask.generated.h"

USTRUCT()
struct CURSEDLANDS_API FCL_ApplyGameplayEffectStateTreeTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<ACharacter> Character;
	
	UPROPERTY(EditDefaultsOnly, Category = "Parameters")
	TSubclassOf<UGameplayEffect> GameplayEffectClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Parameters", Meta = (EditCondition = "GameplayEffectClass != nullptr", EditConditionHides))
	float GameplayEffectLevel = 1.f;
};

USTRUCT(Meta = (DisplayName = "Apply Gameplay Effect"))
struct CURSEDLANDS_API FCL_ApplyGameplayEffectStateTreeTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	FCL_ApplyGameplayEffectStateTreeTask();

	using FInstanceDataType = FCL_ApplyGameplayEffectStateTreeTaskInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif
};
