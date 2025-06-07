// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "StateTreeConditionBase.h"
#include "CLControllerCharacterHasMatchingTagCondition.generated.h"

class ACLPlayerController;

USTRUCT()
struct CURSEDLANDS_API FCL_ControllerCharacterHasMatchingTagConditionInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Context")
	ACLPlayerController* Controller;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	FGameplayTag Tag;
};

USTRUCT(DisplayName = "Controller Character Has Matching Tag")
struct CURSEDLANDS_API FCL_ControllerCharacterHasMatchingTagCondition : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FCL_ControllerCharacterHasMatchingTagConditionInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

#if WITH_EDITOR
	virtual FName GetIconName() const override { return FName("StateTreeEditorStyle|Node.Animation"); }
	virtual FColor GetIconColor() const override { return UE::StateTree::Colors::Grey; }
#endif
};
