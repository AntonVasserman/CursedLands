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

public:
	virtual bool TestCondition(const FCLLandedConditionContext& ConditionContext) const { return false; }
};
