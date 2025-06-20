// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CL_FallingTaskBase.generated.h"


class UCL_CharacterFallingComponent;

USTRUCT(BlueprintType)
struct CURSEDLANDS_API FCL_FallingTaskContext
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACharacter> Character;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCL_CharacterFallingComponent> FallingComponent;
};

UCLASS(Abstract, Blueprintable, EditInlineNew, Meta = (DisplayName = "Falling Task Base"))
class CURSEDLANDS_API UCL_FallingTaskBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void ExecuteTask(const FCL_FallingTaskContext& TaskContext) const;
	virtual void ExecuteTask_Implementation(const FCL_FallingTaskContext& TaskContext) const {}
};
