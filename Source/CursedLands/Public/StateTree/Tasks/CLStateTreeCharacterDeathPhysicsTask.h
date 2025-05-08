// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "StateTreeTaskBase.h"
#include "CLStateTreeCharacterDeathPhysicsTask.generated.h"

class ACLPlayerController;

USTRUCT()
struct CURSEDLANDS_API FCL_StateTreeCharacterDeathPhysicsTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Context")
	ACLPlayerController* Controller;
	
	/** If true the task will run forever until a transition stops it. */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bRunForever = false;
};

USTRUCT(Meta = (DisplayName = "Character Death Physics Task"))
struct FCL_StateTreeCharacterDeathPhysicsTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FCL_StateTreeCharacterDeathPhysicsTaskInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	virtual FName GetIconName() const override { return FName("StateTreeEditorStyle|Node.Animation"); }
	virtual FColor GetIconColor() const override { return UE::StateTree::Colors::Grey; }
#endif
};

