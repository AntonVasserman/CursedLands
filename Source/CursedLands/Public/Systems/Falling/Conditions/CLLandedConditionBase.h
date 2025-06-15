// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLLandedConditionBase.generated.h"\

USTRUCT()
struct CURSEDLANDS_API FCLLandedConditionContext
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<ACharacter> Character;

	UPROPERTY()
	TObjectPtr<AActor> HitActor;

	UPROPERTY()
	float FallHeight;
};

UCLASS(Abstract, EditInlineNew, Meta = (DisplayName = "Landed Condition Base"))
class UCLLandedConditionBase : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (BaseClass = UCLLandedConditionBase), Instanced)
	TObjectPtr<UCLLandedConditionBase> And {nullptr}; 
	
public:
	bool TestCondition(const FCLLandedConditionContext& ConditionContext) const;

protected:
	/**
	 * Evaluates the internal condition based on the provided landing context.
	 * Override this method to implement specific landing condition logic.
	 * This is an example of the Template Method Design Pattern, where we want to high-level logic where we evaluate
	 * "And"s and "Or"s, but each Condition implements its own internal check.
	 *
	 * @param ConditionContext The context of the landing event.
	 * @return True if the condition is satisfied, false otherwise.
	 */
	virtual bool TestConditionInternal(const FCLLandedConditionContext& ConditionContext) const { return false; }
};
