// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeConditionBase.h"
#include "UObject/Object.h"
#include "CL_ObjectImplementsInterfaceStateTreeCondition.generated.h"

USTRUCT()
struct CURSEDLANDS_API FCL_ObjectImplementsInterfaceStateTreeConditionInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Inputs")
	TObjectPtr<UObject> Object;

	UPROPERTY(EditAnywhere, Category = "Parameter", Meta = (AllowAbstract = "true"))
	TSubclassOf<UInterface> Interface;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bInvert = false;
};

USTRUCT(DisplayName = "Object Implements Interface")
struct CURSEDLANDS_API FCL_ObjectImplementsInterfaceStateTreeCondition : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FCL_ObjectImplementsInterfaceStateTreeConditionInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
	
#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif
};
