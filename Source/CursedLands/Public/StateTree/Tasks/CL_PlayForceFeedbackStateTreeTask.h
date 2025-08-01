// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "UObject/Object.h"
#include "CL_PlayForceFeedbackStateTreeTask.generated.h"

USTRUCT()
struct CURSEDLANDS_API FCL_PlayForceFeedbackStateTreeTaskInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<APlayerController> Context;

	UPROPERTY(EditAnywhere, Category = "Parameters", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UForceFeedbackEffect> ForceFeedbackEffect;

	UPROPERTY(EditAnywhere, Category = "Parameters", Meta = (AllowPrivateAccess = "true"))
	FName ForceFeedbackTag;

	UPROPERTY(EditAnywhere, Category = "Parameters", Meta = (AllowPrivateAccess = "true"))
	bool bLoop;
};

USTRUCT(Meta = (DisplayName = "Play Force Feedback"))
struct CURSEDLANDS_API FCL_PlayForceFeedbackStateTreeTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	FCL_PlayForceFeedbackStateTreeTask();
	
	using FInstanceDataType = FCL_PlayForceFeedbackStateTreeTaskInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif
};
