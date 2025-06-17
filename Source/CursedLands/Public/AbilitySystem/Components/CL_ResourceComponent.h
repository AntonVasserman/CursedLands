// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/CL_ResourceAttributeSet.h"
#include "Components/ActorComponent.h"
#include "CL_ResourceComponent.generated.h"

class UCL_AbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCL_OnValueChanged, float, OldValue, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCL_OnValueLimitReached);

UCLASS(Abstract)
class CURSEDLANDS_API UCL_ResourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCL_ResourceComponent();

	UPROPERTY(BlueprintAssignable)
	FCL_OnValueChanged OnValueChanged;

	UPROPERTY(BlueprintAssignable)
	FCL_OnValueChanged OnMaxValueChanged;

	UPROPERTY(BlueprintAssignable)
	FCL_OnValueLimitReached OnResourceDepleted;

	UPROPERTY(BlueprintAssignable)
	FCL_OnValueLimitReached OnResourceFull;

	UFUNCTION(BlueprintCallable, Category = "Resource")
	virtual void InitializeWithAbilitySystem(UCL_AbilitySystemComponent* InAbilitySystemComponent);

	UFUNCTION(BlueprintCallable, Category = "Resource")
	void UnInitializeFromAbilitySystem();
	
	UFUNCTION(BlueprintCallable, Category = "Resource")
	FORCEINLINE float GetValue() const { return ResourceAttributeSet->GetValue(); }

	UFUNCTION(BlueprintCallable, Category = "Resource")
	FORCEINLINE float GetMaxValue() const { return ResourceAttributeSet->GetMaxValue(); }

protected:
	UPROPERTY()
	TObjectPtr<UCL_AbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY()
	TObjectPtr<const UCL_ResourceAttributeSet> ResourceAttributeSet = nullptr;

	TSubclassOf<UCL_ResourceAttributeSet> ResourceAttributeSetClass = nullptr;

	// Abstract functions meant to be implemented by deriving Resource Components
	virtual void ResourceDepletedInternal() {};
	virtual void ResourceFullInternal() {};

private:
	void ResourceDepleted();
	void ResourceFull();
};
