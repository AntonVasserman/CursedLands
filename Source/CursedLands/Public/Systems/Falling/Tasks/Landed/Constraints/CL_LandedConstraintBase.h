// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CL_LandedConstraintBase.generated.h"

USTRUCT(BlueprintType)
struct CURSEDLANDS_API FCL_LandedConstraintContext
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACharacter> Character;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> HitActor;
};

UCLASS(Abstract, Blueprintable, EditInlineNew, Meta = (DisplayName = "Landed Constraint Base"))
class CURSEDLANDS_API UCL_LandedConstraintBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	bool TestConstraint(const FCL_LandedConstraintContext& ConstraintContext) const;
	virtual bool TestConstraint_Implementation(const FCL_LandedConstraintContext& ConstraintContext) const { return false; }
};
