// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/CL_AbilitySystemComponent.h"

#include "CL_LogChannels.h"
#include "AbilitySystem/Abilities/CL_GameplayAbility.h"

void UCL_AbilitySystemComponent::AbilityInputPressed(FGameplayTag InputTag)
{
	if (InputTag.IsValid() == false)
	{
		CL_LOG_GAMEPLAY_ABILITY_SYSTEM(Warning, "Invalid InputTag: %s", *InputTag.ToString())
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability != nullptr && AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
			InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
		}
	}
}

void UCL_AbilitySystemComponent::AbilityInputReleased(FGameplayTag InputTag)
{
	if (InputTag.IsValid() == false)
	{
		CL_LOG_GAMEPLAY_ABILITY_SYSTEM(Warning, "Invalid InputTag: %s", *InputTag.ToString())
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability && AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
			InputHeldSpecHandles.Remove(AbilitySpec.Handle);
		}
	}
}

void UCL_AbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle);
			AbilitySpec->Ability != nullptr && AbilitySpec->IsActive() == false)
		{
			if (const UCL_GameplayAbility* AbilityCDO = Cast<UCL_GameplayAbility>(AbilitySpec->Ability);
				AbilityCDO && AbilityCDO->GetActivationPolicy() == ECL_AbilityActivationPolicy::WhileInputActive)
			{
				AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle);
			AbilitySpec->Ability != nullptr)
		{
			AbilitySpec->InputPressed = true;

			if (AbilitySpec->IsActive())
			{
				AbilitySpecInputPressed(*AbilitySpec);
			}
			else
			{
				if (const UCL_GameplayAbility* AbilityCDO = Cast<UCL_GameplayAbility>(AbilitySpec->Ability);
					AbilityCDO && AbilityCDO->GetActivationPolicy() == ECL_AbilityActivationPolicy::OnInputTriggered)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle);
			AbilitySpec->Ability != nullptr)
		{
			AbilitySpec->InputPressed = false;

			if (AbilitySpec->IsActive())
			{
				AbilitySpecInputReleased(*AbilitySpec);
			}
		}
	}

	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UCL_AbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	if (Spec.IsActive())
	{
		TArray<UGameplayAbility*> Instances = Spec.GetAbilityInstances();
		const FGameplayAbilityActivationInfo& ActivationInfo = Instances.Last()->GetCurrentActivationInfoRef();
		FPredictionKey OriginalPredictionKey = ActivationInfo.GetActivationPredictionKey();
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, OriginalPredictionKey);
	}
}

void UCL_AbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	if (Spec.IsActive())
	{
		TArray<UGameplayAbility*> Instances = Spec.GetAbilityInstances();
		const FGameplayAbilityActivationInfo& ActivationInfo = Instances.Last()->GetCurrentActivationInfoRef();
		FPredictionKey OriginalPredictionKey = ActivationInfo.GetActivationPredictionKey();
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, OriginalPredictionKey);
	}
}

//~ UAbilitySystemComponent Begin
//~ UAbilitySystemComponent End
