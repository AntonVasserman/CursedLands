// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "CL_StateTreeDebugStateTask.generated.h"

USTRUCT()
struct CURSEDLANDS_API FCL_StateTreeDebugStateTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Parameters")
	bool bEnable = false;
};

USTRUCT(Meta = (DisplayName = "Debug State"))
struct CURSEDLANDS_API FCL_StateTreeDebugStateTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	FCL_StateTreeDebugStateTask();

	using FInstanceDataType = FCL_StateTreeDebugStateTaskInstanceData;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	bool bPrintEnter = false;

	UPROPERTY(EditAnywhere, Category = "Parameter", Meta = (EditCondition = "bPrintEnter == true", EditConditionHides))
	FString EnterText;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	bool bPrintExit = false;

	UPROPERTY(EditAnywhere, Category = "Parameter", Meta = (EditCondition = "bPrintExit == true", EditConditionHides))
	FString ExitText;
	
	UPROPERTY(EditAnywhere, Category = "Parameter", Meta = (EditCondition = "bPrintEnter == true || bPrintExit == true", EditConditionHides))
	FColor TextColor = FColor::White;

	UPROPERTY(EditAnywhere, Category = "Parameter", Meta = (ClampMin = 0, UIMin = 0, EditCondition = "bPrintEnter == true || bPrintExit == true", EditConditionHides))
	float FontScale = 1.0f;
	
	//~ Begin FStateTreeTaskCommonBase
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
#if WITH_EDITOR
	virtual FName GetIconName() const override { return FName("StateTreeEditorStyle|Node.Text"); }
	virtual FColor GetIconColor() const override { return UE::StateTree::Colors::Grey; }
#endif
	//~ End FStateTreeTaskCommonBase
};
