// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CL_GASOperationBase.generated.h"

class UAbilitySystemComponent;

UCLASS(Abstract, Blueprintable, EditInlineNew, Meta = (DisplayName = "GAS Operation Base"))
class CURSEDLANDS_API UCL_GASOperationBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void Execute(UAbilitySystemComponent* InAbilitySystemComponent) const;
	virtual void Execute_Implementation(UAbilitySystemComponent* InAbilitySystemComponent) const {}

	UFUNCTION(BlueprintNativeEvent)
	FString GetOperationName() const;
	virtual FString GetOperationName_Implementation() const { return FString(); }
};
