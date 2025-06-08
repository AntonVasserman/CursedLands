// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CLHealthComponent.generated.h"

class UCLAbilitySystemComponent;
class UCLHealthAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCLOnValueChanged, float, OldValue, float, NewValue);

UCLASS()
class CURSEDLANDS_API UCLHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UCLAbilitySystemComponent> AbilitySystemComponent {nullptr};

	UPROPERTY()
	TObjectPtr<const UCLHealthAttributeSet> HealthAttributeSet {nullptr};

public:
	UCLHealthComponent();

	UPROPERTY(BlueprintAssignable)
	FCLOnValueChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FCLOnValueChanged OnMaxHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void InitializeWithAbilitySystem(UCLAbilitySystemComponent* InAbilitySystemComponent);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void UnInitializeFromAbilitySystem();

	//~ UActorComponent Begin
	//~ UActorComponent End
};
