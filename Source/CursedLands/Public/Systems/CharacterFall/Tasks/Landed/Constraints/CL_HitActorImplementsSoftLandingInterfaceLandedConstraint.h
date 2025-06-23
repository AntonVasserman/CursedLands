// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Tasks/Landed/Constraints/AV_LandedConstraintBase.h"
#include "CL_HitActorImplementsSoftLandingInterfaceLandedConstraint.generated.h"

UENUM()
enum class ECL_ImplementsInterfaceOption : uint8
{
	MustImplement		UMETA(DisplayName = "Must Implement"),
	MustNotImplement	UMETA(DisplayName = "Must Not Implement"),
};

UCLASS(Meta = (DisplayName = "Hit Actor Implements Soft Landing Interface"))
class CURSEDLANDS_API UCL_HitActorImplementsSoftLandingInterfaceLandedConstraint : public UAV_LandedConstraintBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (AllowPrivateAccess = "true"))
	ECL_ImplementsInterfaceOption ImplementsOption = ECL_ImplementsInterfaceOption::MustImplement;
	
	//~ UAV_LandedConstraintBase Begin
public:
	virtual bool TestConstraint_Implementation(const FAV_LandedConstraintContext& ConstraintContext) const override;
	//~ UAV_LandedConstraintBase End
};
