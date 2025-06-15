// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Systems/Falling/Conditions/CLFallHeightInRangeLandedCondition.h"
#include "CLLandedTaskBase.generated.h"

class UCLCharacterFallingComponent;

USTRUCT()
struct CURSEDLANDS_API FCLLandedTaskContext
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UCLLandedConditionBase> Condition;

	UPROPERTY()
	float FallHeight;
	
	UPROPERTY()
	TObjectPtr<ACharacter> Character;

	UPROPERTY()
	TObjectPtr<UCLCharacterFallingComponent> FallingComponent;
};

UCLASS(Abstract, EditInlineNew, Meta = (DisplayName = "Landed Task Base"))
class CURSEDLANDS_API UCLLandedTaskBase : public UObject
{
	GENERATED_BODY()
	
public:	
	virtual void ExecuteTask(const FCLLandedTaskContext& TaskContext) const {}
};
