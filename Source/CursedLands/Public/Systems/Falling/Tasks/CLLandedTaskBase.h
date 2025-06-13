// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Systems/Falling/Conditions/CLCheckFallHeightLandedCondition.h"
#include "CLLandedTaskBase.generated.h"

class UCLCharacterFallingComponent;

USTRUCT()
struct CURSEDLANDS_API FCLLandedTaskContext
{
	GENERATED_BODY()

	UPROPERTY()
	FCLCheckFallHeightLandedCondition Condition;

	UPROPERTY()
	float FallHeight;
	
	UPROPERTY()
	TObjectPtr<ACharacter> Character;

	UPROPERTY()
	TObjectPtr<UCLCharacterFallingComponent> FallingComponent;
};

UCLASS(Blueprintable, EditInlineNew)
class CURSEDLANDS_API UCLLandedTaskBase : public UObject
{
	GENERATED_BODY()
	
public:	
	virtual void ExecuteTask(const FCLLandedTaskContext& TaskContext) const {}
};
