// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AV_LandedConstraintBase.generated.h"

class AActor;
class ACharacter;

USTRUCT(BlueprintType)
struct AVCHARACTERFALL_API FAV_LandedConstraintContext
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "")
	TObjectPtr<ACharacter> Character = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "")
	TObjectPtr<AActor> HitActor = nullptr;
};

UCLASS(Abstract, Blueprintable, EditInlineNew, Category = "Character Fall|Tasks|Landed|Constraints", Meta = (DisplayName = "Landed Constraint Base"))
class AVCHARACTERFALL_API UAV_LandedConstraintBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Character Fall|Tasks|Landed|Constraints")
	bool TestConstraint(const FAV_LandedConstraintContext& ConstraintContext) const;
	virtual bool TestConstraint_Implementation(const FAV_LandedConstraintContext& ConstraintContext) const { return false; }
};
