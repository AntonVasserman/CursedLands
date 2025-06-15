// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLLandedConditionBase.h"
#include "Engine/DataAsset.h"
#include "CLHitActorImplementsInterfaceLandedCondition.generated.h"

struct FCLLandedConditionContext;

UCLASS(EditInlineNew, Meta = (DisplayName = "Hit Actor Implements Interface"))
class CURSEDLANDS_API UCLHitActorImplementsInterfaceLandedCondition : public UCLLandedConditionBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UInterface> InterfaceClass {nullptr};

	//~ UCLLandedConditionBase Begin
public:
	virtual bool TestCondition(const FCLLandedConditionContext& ConditionContext) const override;
	//~ UCLLandedConditionBase End
};
