// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_LandedConditionBase.h"
#include "Engine/DataAsset.h"
#include "CL_HitActorImplementsSoftLandingInterfaceLandedCondition.generated.h"

struct FCL_LandedConditionContext;

UENUM()
enum class ECL_ImplementsInterfaceOption : uint8
{
	MustImplement		UMETA(DisplayName = "Must Implement"),
	MustNotImplement	UMETA(DisplayName = "Must Not Implement"),
};

UCLASS(EditInlineNew, Meta = (DisplayName = "Hit Actor Implements Soft Landing Interface"))
class CURSEDLANDS_API UCL_HitActorImplementsSoftLandingInterfaceLandedCondition : public UCL_LandedConditionBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (AllowPrivateAccess = "true"))
	ECL_ImplementsInterfaceOption ImplementsOption = ECL_ImplementsInterfaceOption::MustImplement;

	//~ UCLLandedConditionBase Begin
protected:
	virtual bool TestConditionInternal_Implementation(const FCL_LandedConditionContext& ConditionContext) const override;
	//~ UCLLandedConditionBase End
};
