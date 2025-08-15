// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "UObject/Object.h"
#include "CL_MotionMatchAndPlayAnimMontageStateTreeTask.generated.h"

USTRUCT()
struct CURSEDLANDS_API FCL_MotionMatchAndPlayAnimMontageStateTreeTaskInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<ACharacter> Character;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	TArray<UAnimMontage*> AnimMontages;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	bool bJumpToSection = false;

	UPROPERTY(EditAnywhere, Category = "Parameters", Meta = (EditCondition = "bJumpToSection == true", EditConditionHides))
	FName SectionName = NAME_None;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	FName PoseHistoryName = NAME_None;
};


USTRUCT(Meta = (DisplayName = "Motion Match and Play Anim Montage"))
struct CURSEDLANDS_API FCL_MotionMatchAndPlayAnimMontageStateTreeTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	FCL_MotionMatchAndPlayAnimMontageStateTreeTask();
	
	using FInstanceDataType = FCL_MotionMatchAndPlayAnimMontageStateTreeTaskInstanceData;

	//~ Begin FStateTreeTaskCommonBase
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual EDataValidationResult Compile(UE::StateTree::ICompileNodeContext& CompileContext) override;
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif
	//~ End FStateTreeTaskCommonBase
};
