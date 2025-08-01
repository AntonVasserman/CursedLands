// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "CL_PlayAnimMontageStateTreeTask.generated.h"

USTRUCT()
struct CURSEDLANDS_API FCL_PlayAnimMontageStateTreeTaskInstanceData : public FStateTreeTaskBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<APlayerController> Context;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	TObjectPtr<UAnimMontage> AnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Parameters", Meta = (EditCondition = "AnimMontage != nullptr", EditConditionHides))
	bool bJumpToSection = false;

	UPROPERTY(EditAnywhere, Category = "Parameters", Meta = (EditCondition = "AnimMontage != nullptr && bJumpToSection == true", EditConditionHides))
	FName SectionName;
};

USTRUCT(Meta = (DisplayName = "Play Anim Montage"))
struct CURSEDLANDS_API FCL_PlayAnimMontageStateTreeTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	FCL_PlayAnimMontageStateTreeTask();
	
	using FInstanceDataType = FCL_PlayAnimMontageStateTreeTaskInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif
};
