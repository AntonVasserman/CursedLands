// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/CLStaminaAttributeSet.h"
#include "Components/ActorComponent.h"
#include "CLStaminaComponent.generated.h"

class UCLAbilitySystemComponent;
class UCLStaminaAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCLOnValueChangedTemp, float, OldValue, float, NewValue);

UCLASS()
class CURSEDLANDS_API UCLStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UCLAbilitySystemComponent> AbilitySystemComponent {nullptr};

	UPROPERTY()
	TObjectPtr<const UCLStaminaAttributeSet> StaminaAttributeSet {nullptr};
	
public:
	UCLStaminaComponent();

	UPROPERTY(BlueprintAssignable)
	FCLOnValueChangedTemp OnStaminaChanged;

	UPROPERTY(BlueprintAssignable)
	FCLOnValueChangedTemp OnMaxStaminaChanged;

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void InitializeWithAbilitySystem(UCLAbilitySystemComponent* InAbilitySystemComponent);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void UnInitializeFromAbilitySystem();

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	FORCEINLINE float GetStamina() const { return StaminaAttributeSet->GetStamina(); }

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	FORCEINLINE float GetMaxStamina() const { return StaminaAttributeSet->GetMaxStamina(); }

	//~ UActorComponent Begin
	//~ UActorComponent End
};
