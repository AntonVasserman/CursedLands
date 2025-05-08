// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "StateTreeTaskBase.h"
#include "CLStateTreePrintStringTask.generated.h"

USTRUCT()
struct CURSEDLANDS_API FCL_StateTreePrintStringTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Parameter")
	FString StringToPrint;

	/** If true the task will run forever until a transition stops it. */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bRunForever = false;
};

// This is a temporary debug task used to output string so I can test StateTree behavior.
// NOTE: I'm hashing the provided string to generate a key, meaning that two equivalent strings will result in message overriding.
USTRUCT(Meta = (DisplayName = "Print String Task"))
struct CURSEDLANDS_API FCL_StateTreePrintStringTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FCL_StateTreePrintStringTaskInstanceData;

	FCL_StateTreePrintStringTask() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	
#if WITH_EDITOR
	virtual FName GetIconName() const override { return FName("StateTreeEditorStyle|Node.Text"); }
	virtual FColor GetIconColor() const override { return UE::StateTree::Colors::Grey; }
#endif
};
