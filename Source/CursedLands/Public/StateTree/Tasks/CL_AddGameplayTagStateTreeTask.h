// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "UObject/Object.h"
#include "CL_AddGameplayTagStateTreeTask.generated.h"

USTRUCT()
struct CURSEDLANDS_API FCL_AddGameplayTagStateTreeTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<AActor> Actor;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	FGameplayTag GameplayTag;
};

USTRUCT(Meta = (DisplayName = "Add Gameplay Tag"))
struct CURSEDLANDS_API FCL_AddGameplayTagStateTreeTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	FCL_AddGameplayTagStateTreeTask();
	
	using FInstanceDataType = FCL_AddGameplayTagStateTreeTaskInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	bool bRemoveOnExit = false;
};
