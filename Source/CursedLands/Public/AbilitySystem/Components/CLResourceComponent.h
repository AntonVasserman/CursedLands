// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/CLResourceAttributeSet.h"
#include "Components/ActorComponent.h"
#include "CLResourceComponent.generated.h"

class UCLAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCLOnValueChanged, float, OldValue, float, NewValue);

UCLASS(Abstract)
class CURSEDLANDS_API UCLResourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCLResourceComponent();

	UPROPERTY(BlueprintAssignable)
	FCLOnValueChanged OnValueChanged;

	UPROPERTY(BlueprintAssignable)
	FCLOnValueChanged OnMaxValueChanged;

	UFUNCTION(BlueprintCallable, Category = "Resource")
	void InitializeWithAbilitySystem(UCLAbilitySystemComponent* InAbilitySystemComponent);

	UFUNCTION(BlueprintCallable, Category = "Resource")
	void UnInitializeFromAbilitySystem();
	
	UFUNCTION(BlueprintCallable, Category = "Resource")
	FORCEINLINE float GetValue() const { return ResourceAttributeSet->GetValue(); }

	UFUNCTION(BlueprintCallable, Category = "Resource")
	FORCEINLINE float GetMaxValue() const { return ResourceAttributeSet->GetMaxValue(); }

protected:
	UPROPERTY()
	TObjectPtr<UCLAbilitySystemComponent> AbilitySystemComponent {nullptr};

	UPROPERTY()
	TObjectPtr<const UCLResourceAttributeSet> ResourceAttributeSet {nullptr};

	TSubclassOf<UCLResourceAttributeSet> ResourceAttributeSetClass {nullptr};
};
