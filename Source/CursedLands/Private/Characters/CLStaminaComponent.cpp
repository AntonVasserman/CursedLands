// Copyright Anton Vasserman, All Rights Reserved.

#include "Characters/CLStaminaComponent.h"

#include "AbilitySystem/CLAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/CLStaminaAttributeSet.h"

UCLStaminaComponent::UCLStaminaComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UCLStaminaComponent::InitializeWithAbilitySystem(UCLAbilitySystemComponent* InAbilitySystemComponent)
{
	AActor* OwningActor {GetOwner()};
	check(OwningActor);

	checkf(AbilitySystemComponent == nullptr, TEXT("CLStaminaComponent::%hs StaminaComponent for owner %s has already been initialized with an ability system"), __FUNCTION__, *OwningActor->GetName());
	checkf(InAbilitySystemComponent != nullptr, TEXT("CLStaminaComponent::%hs Cannot initialize StaminaComponent for owner %s, with null input ability system!"), __FUNCTION__, *OwningActor->GetName());
	
	AbilitySystemComponent = InAbilitySystemComponent;
	StaminaAttributeSet = AbilitySystemComponent->GetSet<UCLStaminaAttributeSet>();

	checkf(StaminaAttributeSet != nullptr, TEXT("CLStaminaComponent::%hs Cannot initialize StaminaComponent for owner %s, with null StaminaAttributeSet on the ability system."), __FUNCTION__, *OwningActor->GetName());

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(StaminaAttributeSet->GetStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnStaminaChanged.Broadcast(Data.OldValue, Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(StaminaAttributeSet->GetMaxStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxStaminaChanged.Broadcast(Data.OldValue, Data.NewValue);
		});
}

void UCLStaminaComponent::UnInitializeFromAbilitySystem()
{
	if (StaminaAttributeSet != nullptr)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(StaminaAttributeSet->GetStaminaAttribute()).RemoveAll(this);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(StaminaAttributeSet->GetMaxStaminaAttribute()).RemoveAll(this);
		
		StaminaAttributeSet = nullptr;
	}
	
	AbilitySystemComponent = nullptr;
}

//~ UActorComponent Begin
//~ UActorComponent End
