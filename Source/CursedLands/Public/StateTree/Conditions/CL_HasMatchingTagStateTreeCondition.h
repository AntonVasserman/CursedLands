// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "StateTreeConditionBase.h"
#include "CL_HasMatchingTagStateTreeCondition.generated.h"

USTRUCT()
struct CURSEDLANDS_API FCL_HasMatchingTagStateTreeConditionInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<AActor> Actor;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	FGameplayTag Tag;
};

USTRUCT(DisplayName = "Has Matching Tag")
struct CURSEDLANDS_API FCL_HasMatchingTagStateTreeCondition : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FCL_HasMatchingTagStateTreeConditionInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

#if WITH_EDITOR
	virtual FName GetIconName() const override { return FName("StateTreeEditorStyle|Node.Animation"); }
	virtual FColor GetIconColor() const override { return UE::StateTree::Colors::Grey; }
#endif
};
