// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLLandedConditionBase.h"
#include "Engine/DataAsset.h"
#include "CLHitActorImplementsSoftLandingInterfaceLandedCondition.generated.h"

struct FCLLandedConditionContext;

UENUM()
enum class ECLImplementsInterfaceOption : uint8
{
	MustImplement		UMETA(DisplayName = "Must Implement"),
	MustNotImplement	UMETA(DisplayName = "Must Not Implement"),
};

UCLASS(EditInlineNew, Meta = (DisplayName = "Hit Actor Implements Soft Landing Interface"))
class CURSEDLANDS_API UCLHitActorImplementsSoftLandingInterfaceLandedCondition : public UCLLandedConditionBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (AllowPrivateAccess = "true"))
	ECLImplementsInterfaceOption ImplementsOption = ECLImplementsInterfaceOption::MustImplement;

	//~ UCLLandedConditionBase Begin
protected:
	virtual bool TestConditionInternal_Implementation(const FCLLandedConditionContext& ConditionContext) const override;
	//~ UCLLandedConditionBase End
};
