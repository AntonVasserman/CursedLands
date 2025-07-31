// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "UObject/Object.h"
#include "CL_GameplayTagTask.generated.h"

class UCL_GASOperationBase;
class ACL_PlayerController;

USTRUCT()
struct CURSEDLANDS_API FCL_GameplayTagTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<ACL_PlayerController> Controller;

	UPROPERTY(EditAnywhere, Category = "", Meta = (AllowPrivateAccess = "true"))
	FGameplayTag GameplayTag;
};

USTRUCT(Meta = (DisplayName = "Gameplay Tag StateTree Task"))
struct CURSEDLANDS_API FCL_GameplayTagTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	FCL_GameplayTagTask();
	
	using FInstanceDataType = FCL_GameplayTagTaskInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
