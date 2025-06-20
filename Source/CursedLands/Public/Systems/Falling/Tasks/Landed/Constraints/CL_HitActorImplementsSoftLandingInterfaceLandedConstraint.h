// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_LandedConstraintBase.h"
#include "Engine/DataAsset.h"
#include "CL_HitActorImplementsSoftLandingInterfaceLandedConstraint.generated.h"

struct FCL_LandedConditionContext;

UENUM()
enum class ECL_ImplementsInterfaceOption : uint8
{
	MustImplement		UMETA(DisplayName = "Must Implement"),
	MustNotImplement	UMETA(DisplayName = "Must Not Implement"),
};

UCLASS(EditInlineNew, Meta = (DisplayName = "Hit Actor Implements Soft Landing Interface"))
class CURSEDLANDS_API UCL_HitActorImplementsSoftLandingInterfaceLandedConstraint : public UCL_LandedConstraintBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (AllowPrivateAccess = "true"))
	ECL_ImplementsInterfaceOption ImplementsOption = ECL_ImplementsInterfaceOption::MustImplement;

	//~ UCL_LandedConstraintBase Begin
protected:
	virtual bool TestConstraint_Implementation(const FCL_LandedConstraintContext& ConstraintContext) const override;
	//~ UCL_LandedConstraintBase End
};