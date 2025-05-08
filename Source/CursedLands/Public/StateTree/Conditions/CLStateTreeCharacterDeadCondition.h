// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "StateTreeConditionBase.h"
#include "CLStateTreeCharacterDeadCondition.generated.h"

class ACLPlayerController;

USTRUCT()
struct FCL_StateTreeCharacterDeadConditionInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Context")
	ACLPlayerController* Controller;
};

USTRUCT(DisplayName = "Character Dead Condition")
struct CURSEDLANDS_API FCL_StateTreeCharacterDeadCondition : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FCL_StateTreeCharacterDeadConditionInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

#if WITH_EDITOR
	virtual FName GetIconName() const override { return FName("StateTreeEditorStyle|Node.Animation"); }
	virtual FColor GetIconColor() const override { return UE::StateTree::Colors::Grey; }
#endif
};