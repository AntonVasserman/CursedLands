// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_LandedConditionBase.generated.h"
USTRUCT(BlueprintType)
struct CURSEDLANDS_API FCL_LandedConditionContext
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACharacter> Character;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> HitActor;

	UPROPERTY(BlueprintReadOnly)
	float FallHeight = 0.f;
};

UCLASS(Abstract, Blueprintable, EditInlineNew, Meta = (DisplayName = "Landed Condition Base"))
class UCL_LandedConditionBase : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (BaseClass = UCLLandedConditionBase), Instanced)
	TObjectPtr<UCL_LandedConditionBase> And = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (BaseClass = UCLLandedConditionBase), Instanced)
	TObjectPtr<UCL_LandedConditionBase> Or = nullptr;
	
public:
	UFUNCTION(BlueprintCallable)
	bool TestCondition(const FCL_LandedConditionContext& ConditionContext) const;

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
	UFUNCTION(BlueprintNativeEvent)
	bool TestConditionInternal(const FCL_LandedConditionContext& ConditionContext) const;
	virtual bool TestConditionInternal_Implementation(const FCL_LandedConditionContext& ConditionContext) const { return false; }
};
