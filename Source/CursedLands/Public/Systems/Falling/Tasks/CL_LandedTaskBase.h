// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_LandedTaskBase.generated.h"

class UCL_LandedConstraintBase;
class UCL_CharacterFallingComponent;

USTRUCT(BlueprintType)
struct CURSEDLANDS_API FCL_LandedTaskContext
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACharacter> Character;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> HitActor;
	
	UPROPERTY(BlueprintReadOnly)
	float FallHeight = 0.f;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCL_CharacterFallingComponent> FallingComponent;
};

UCLASS(Abstract, Blueprintable, EditInlineNew, Meta = (DisplayName = "Landed Task Base"))
class CURSEDLANDS_API UCL_LandedTaskBase : public UObject
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "")
	uint8 bHasConstraint:1 = false;

	UPROPERTY(EditDefaultsOnly, Category = "", Instanced, Meta = (BaseClass = UCL_LandedConstraintBase, EditCondition = "bHasConstraint == true", EditConditionHides))
	TArray<TObjectPtr<UCL_LandedConstraintBase>> Constraints;
	
public:
	UFUNCTION(BlueprintCallable)
	void ExecuteTask(const FCL_LandedTaskContext& TaskContext) const;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void ExecuteTaskInternal(const FCL_LandedTaskContext& TaskContext) const;
	virtual void ExecuteTaskInternal_Implementation(const FCL_LandedTaskContext& TaskContext) const {}
};
