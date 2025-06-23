// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AV_FallingTaskBase.generated.h"

class ACharacter;
class UAV_CharacterFallComponent;

USTRUCT(BlueprintType)
struct AVCHARACTERFALL_API FAV_FallingTaskContext
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "")
	TObjectPtr<ACharacter> Character = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "")
	TObjectPtr<UAV_CharacterFallComponent> FallingComponent = nullptr;
};

UCLASS(Abstract, Blueprintable, EditInlineNew, Category = "Character Fall|Tasks|Falling", Meta = (DisplayName = "Falling Task Base"))
class AVCHARACTERFALL_API UAV_FallingTaskBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Character Fall|Tasks|Falling")
	void ExecuteTask(const FAV_FallingTaskContext& TaskContext) const;
	virtual void ExecuteTask_Implementation(const FAV_FallingTaskContext& TaskContext) const {}
};
