// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Components/CLResourceComponent.h"

#include "AbilitySystem/CLAbilitySystemComponent.h"

UCLResourceComponent::UCLResourceComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UCLResourceComponent::InitializeWithAbilitySystem(UCLAbilitySystemComponent* InAbilitySystemComponent)
{
	const AActor* OwningActor = GetOwner();
	check(OwningActor);
	checkf(AbilitySystemComponent == nullptr, TEXT("%s::%hs: %s for owner %s has already been initialized with an ability system"), *GetClass()->GetName(), __FUNCTION__, *ResourceAttributeSet.GetClass()->GetName(), *OwningActor->GetName());
	checkf(InAbilitySystemComponent != nullptr, TEXT("%s::%hs: Cannot initialize %s for owner %s, with null input ability system!"), *GetClass()->GetName(), __FUNCTION__, *ResourceAttributeSet.GetClass()->GetName(), *OwningActor->GetName());
	
	AbilitySystemComponent = InAbilitySystemComponent;
	checkf(ResourceAttributeSetClass != nullptr, TEXT("%s::%hs: Cannot initialize %s for owner %s, because ResourceAttributeSetClass is not set!"), *GetClass()->GetName(), __FUNCTION__, *ResourceAttributeSet.GetClass()->GetName(), *OwningActor->GetName());
	ResourceAttributeSet = Cast<UCLResourceAttributeSet>(AbilitySystemComponent->GetAttributeSet(ResourceAttributeSetClass));

	checkf(ResourceAttributeSet != nullptr, TEXT("%s::%hs: Cannot initialize %s for owner %s, with null HealthAttributeSet on the ability system."), *GetClass()->GetName(), __FUNCTION__, *ResourceAttributeSet.GetClass()->GetName(), *OwningActor->GetName());
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ResourceAttributeSet->GetValueAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnValueChanged.Broadcast(Data.OldValue, Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ResourceAttributeSet->GetMaxValueAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxValueChanged.Broadcast(Data.OldValue, Data.NewValue);
		});
}

void UCLResourceComponent::UnInitializeFromAbilitySystem()
{
	if (ResourceAttributeSet != nullptr)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ResourceAttributeSet->GetValueAttribute()).RemoveAll(this);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ResourceAttributeSet->GetMaxValueAttribute()).RemoveAll(this);
		
		ResourceAttributeSet = nullptr;
	}
	
	AbilitySystemComponent = nullptr;
}

