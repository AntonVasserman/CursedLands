// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AV_LandedTaskBase.generated.h"

class AActor;
class ACharacter;
class UAV_CharacterFallComponent;
class UAV_LandedConstraintBase;

USTRUCT(BlueprintType)
struct AVCHARACTERFALL_API FAV_LandedTaskContext
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "")
	TObjectPtr<ACharacter> Character;
	
	UPROPERTY(BlueprintReadOnly, Category = "")
	TObjectPtr<AActor> HitActor;
	
	UPROPERTY(BlueprintReadOnly, Category = "")
	float FallHeight = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "")
	TObjectPtr<UAV_CharacterFallComponent> FallingComponent;
};

UCLASS(Abstract, Blueprintable, EditInlineNew, Category = "Character Fall|Tasks|Landed", Meta = (DisplayName = "Landed Task Base"))
class AVCHARACTERFALL_API UAV_LandedTaskBase : public UObject
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "")
	uint8 bHasConstraint:1 = false;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "", Meta = (BaseClass = UAV_LandedConstraintBase, EditCondition = "bHasConstraint == true", EditConditionHides))
	TArray<TObjectPtr<UAV_LandedConstraintBase>> Constraints;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Character Fall|Tasks|Landed")
	void ExecuteTask(const FAV_LandedTaskContext& TaskContext) const;

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Character Fall|Tasks|Landed")
	void ExecuteTaskInternal(const FAV_LandedTaskContext& TaskContext) const;
	virtual void ExecuteTaskInternal_Implementation(const FAV_LandedTaskContext& TaskContext) const {}
};
