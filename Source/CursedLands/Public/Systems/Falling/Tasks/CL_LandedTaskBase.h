// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Systems/Falling/Conditions/CL_FallHeightInRangeLandedCondition.h"
#include "CL_LandedTaskBase.generated.h"

class UCL_CharacterFallingComponent;

USTRUCT(BlueprintType)
struct CURSEDLANDS_API FCL_LandedTaskContext
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCL_LandedConditionBase> Condition;

	UPROPERTY(BlueprintReadOnly)
	float FallHeight = 0.f;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACharacter> Character;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCL_CharacterFallingComponent> FallingComponent;
};

UCLASS(Abstract, Blueprintable, EditInlineNew, Meta = (DisplayName = "Landed Task Base"))
class CURSEDLANDS_API UCL_LandedTaskBase : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void ExecuteTask(const FCL_LandedTaskContext& TaskContext) const;
	virtual void ExecuteTask_Implementation(const FCL_LandedTaskContext& TaskContext) const {}
};
