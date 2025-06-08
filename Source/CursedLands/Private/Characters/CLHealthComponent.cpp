// Copyright Anton Vasserman, All Rights Reserved.

#include "Characters/CLHealthComponent.h"

#include "CLLogChannels.h"
#include "AbilitySystem/CLAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/CLHealthAttributeSet.h"

UCLHealthComponent::UCLHealthComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UCLHealthComponent::InitializeWithAbilitySystem(UCLAbilitySystemComponent* InAbilitySystemComponent)
{
	AActor* OwningActor {GetOwner()};
	check(OwningActor);

	checkf(AbilitySystemComponent == nullptr, TEXT("CLHealthComponent::%hs HealthComponent for owner %s has already been initialized with an ability system"), __FUNCTION__, *OwningActor->GetName());
	checkf(InAbilitySystemComponent != nullptr, TEXT("CLHealthComponent::%hs Cannot initialize HealthComponent for owner %s, with null input ability system!"), __FUNCTION__, *OwningActor->GetName());
	
	AbilitySystemComponent = InAbilitySystemComponent;
	HealthAttributeSet = AbilitySystemComponent->GetSet<UCLHealthAttributeSet>();

	checkf(HealthAttributeSet != nullptr, TEXT("CLHealthComponent::%hs Cannot initialize HealthComponent for owner %s, with null HealthAttributeSet on the ability system."), __FUNCTION__, *OwningActor->GetName());
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.OldValue, Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.OldValue, Data.NewValue);
		});
}

void UCLHealthComponent::UnInitializeFromAbilitySystem()
{
	if (HealthAttributeSet != nullptr)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetHealthAttribute()).RemoveAll(this);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetMaxHealthAttribute()).RemoveAll(this);
		
		HealthAttributeSet = nullptr;
	}
	
	AbilitySystemComponent = nullptr;
}

//~ UActorComponent Begin
//~ UActorComponent End
