// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Systems/Falling/Conditions/CLFallHeightInRangeLandedCondition.h"
#include "CLLandedTaskBase.generated.h"

class UCLCharacterFallingComponent;

USTRUCT(BlueprintType)
struct CURSEDLANDS_API FCLLandedTaskContext
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCLLandedConditionBase> Condition;

	UPROPERTY(BlueprintReadOnly)
	float FallHeight;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACharacter> Character;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCLCharacterFallingComponent> FallingComponent;
};

UCLASS(Abstract, Blueprintable, EditInlineNew, Meta = (DisplayName = "Landed Task Base"))
class CURSEDLANDS_API UCLLandedTaskBase : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void ExecuteTask(const FCLLandedTaskContext& TaskContext) const;
	virtual void ExecuteTask_Implementation(const FCLLandedTaskContext& TaskContext) const {}
};
