// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Components/CL_ResourceComponent.h"

#include "AbilitySystem/CL_AbilitySystemComponent.h"
#include "MVVMGameSubsystem.h"
#include "MVVMSubsystem.h"
#include "UI/ViewModels/CL_ResourceViewModel.h"

UCL_ResourceComponent::UCL_ResourceComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UCL_ResourceComponent::InitializeWithAbilitySystem(UCL_AbilitySystemComponent* InAbilitySystemComponent)
{
	const AActor* OwningActor = GetOwner();
	check(OwningActor);
	checkf(AbilitySystemComponent == nullptr, TEXT("%s::%hs: %s for owner %s has already been initialized with an ability system"), *GetClass()->GetName(), __FUNCTION__, *ResourceAttributeSet.GetClass()->GetName(), *OwningActor->GetName());
	checkf(InAbilitySystemComponent != nullptr, TEXT("%s::%hs: Cannot initialize %s for owner %s, with null input ability system!"), *GetClass()->GetName(), __FUNCTION__, *ResourceAttributeSet.GetClass()->GetName(), *OwningActor->GetName());
	
	AbilitySystemComponent = InAbilitySystemComponent;
	checkf(ResourceAttributeSetClass != nullptr, TEXT("%s::%hs: Cannot initialize %s for owner %s, because ResourceAttributeSetClass is not set!"), *GetClass()->GetName(), __FUNCTION__, *ResourceAttributeSet.GetClass()->GetName(), *OwningActor->GetName());
	ResourceAttributeSet = Cast<UCL_ResourceAttributeSet>(AbilitySystemComponent->GetAttributeSet(ResourceAttributeSetClass));

	checkf(ResourceAttributeSet != nullptr, TEXT("%s::%hs: Cannot initialize %s for owner %s, with null ResourceAttributeSet on the ability system."), *GetClass()->GetName(), __FUNCTION__, *ResourceAttributeSet.GetClass()->GetName(), *OwningActor->GetName());
	
	AbilitySystemComponent->AddLooseGameplayTag(ResourceGameplayTags.Full);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ResourceAttributeSet->GetValueAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			const ECL_ResourceState OldState = EvaluateResourceState(Data.OldValue);
			const ECL_ResourceState NewState = EvaluateResourceState(Data.NewValue);
			if (OldState != NewState)
			{
				AbilitySystemComponent->RemoveLooseGameplayTag(GetResourceGameplayTag(OldState));
				AbilitySystemComponent->AddLooseGameplayTag(GetResourceGameplayTag(NewState));
				ResourceStateChanged(OldState, NewState);
			}

			if (ResourceViewModel != nullptr)
			{
				ResourceViewModel->SetCurrentValue(Data.NewValue);
			}
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ResourceAttributeSet->GetMaxValueAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			if (ResourceViewModel != nullptr)
			{
				ResourceViewModel->SetMaxValue(Data.NewValue);
			}
		});
}

void UCL_ResourceComponent::UnInitializeFromAbilitySystem()
{
	if (ResourceAttributeSet != nullptr)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ResourceAttributeSet->GetValueAttribute()).RemoveAll(this);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ResourceAttributeSet->GetMaxValueAttribute()).RemoveAll(this);
		
		ResourceAttributeSet = nullptr;
	}
	
	AbilitySystemComponent = nullptr;
}

void UCL_ResourceComponent::InitializeViewModel()
{
	const UMVVMGameSubsystem* ViewModelGameSubsystem = GetOwner()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>();
	check(ViewModelGameSubsystem);

	UMVVMViewModelCollectionObject* GlobalViewModelCollection = ViewModelGameSubsystem->GetViewModelCollection();
	check(GlobalViewModelCollection);
	
	FMVVMViewModelContext NewResourceViewModelContext;
	NewResourceViewModelContext.ContextClass = UCL_ResourceViewModel::StaticClass();
	NewResourceViewModelContext.ContextName = ResourceViewModelContextName;
	
	if (NewResourceViewModelContext.IsValid())
	{
		UCL_ResourceViewModel* NewResourceViewModel = NewObject<UCL_ResourceViewModel>();
		NewResourceViewModel->SetCurrentValue(ResourceAttributeSet->GetValue());
		NewResourceViewModel->SetMaxValue(ResourceAttributeSet->GetMaxValue());

		ResourceViewModel = NewResourceViewModel;
		GlobalViewModelCollection->AddViewModelInstance(NewResourceViewModelContext, NewResourceViewModel);
		
		OnResourceViewModelInstantiated.Broadcast(NewResourceViewModel);
	}
}

ECL_ResourceState UCL_ResourceComponent::EvaluateResourceState(float Value) const
{
	if (Value == 0.f)
	{
		return ECL_ResourceState::Depleted;
	}
	
	if (Value == ResourceAttributeSet->GetMaxValue())
	{
		return ECL_ResourceState::Full;
	}
	
	const float ValuePercentage = Value / ResourceAttributeSet->GetMaxValue();
	return ValuePercentage > CriticalSectionThresholdInPercentage ? ECL_ResourceState::Normal : ECL_ResourceState::Critical;
}

FGameplayTag UCL_ResourceComponent::GetResourceGameplayTag(ECL_ResourceState State) const
{
	switch (State)
	{
	case ECL_ResourceState::Full:
		return ResourceGameplayTags.Full;
	case ECL_ResourceState::Normal:
		return ResourceGameplayTags.Normal;
	case ECL_ResourceState::Critical:
		return ResourceGameplayTags.Critical;
	case ECL_ResourceState::Depleted:
		return ResourceGameplayTags.Depleted;
	default:
		checkNoEntry();
		return FGameplayTag();
	};
}

