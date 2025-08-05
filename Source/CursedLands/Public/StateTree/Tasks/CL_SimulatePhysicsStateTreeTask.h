// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "StateTreeTaskBase.h"
#include "CL_SimulatePhysicsStateTreeTask.generated.h"

USTRUCT()
struct CURSEDLANDS_API FCL_SimulatePhysicsStateTreeTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<ACharacter> Character;
};

USTRUCT(Meta = (DisplayName = "Simulate Physics"))
struct CURSEDLANDS_API FCL_SimulatePhysicsStateTreeTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FCL_SimulatePhysicsStateTreeTaskInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual FName GetIconName() const override { return FName("StateTreeEditorStyle|Node.Animation"); }
	virtual FColor GetIconColor() const override { return UE::StateTree::Colors::Grey; }
#endif
};

