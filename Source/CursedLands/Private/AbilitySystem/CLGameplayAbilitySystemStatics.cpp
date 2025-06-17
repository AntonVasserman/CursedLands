// Copyright Anton Vasserman, All Rights Reserved.

#include "AbilitySystem/CLGameplayAbilitySystemStatics.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"

TOptional<UAbilitySystemComponent*> UCLGameplayAbilitySystemStatics::GetAbilitySystemComponent(const ACharacter* Character)
{
	// Try to look for the AbilitySystemComponent by casting to IAbilitySystemInterface
	if (const IAbilitySystemInterface* CharacterAsAbilitySystemInterface = Cast<IAbilitySystemInterface>(Character);
		CharacterAsAbilitySystemInterface != nullptr)
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = CharacterAsAbilitySystemInterface->GetAbilitySystemComponent();
			AbilitySystemComponent != nullptr)
		{
			return AbilitySystemComponent;
		}
	}

	// Try to look for the AbilitySystemComponent by looking for it
	if (UAbilitySystemComponent* AbilitySystemComponent = Character->FindComponentByClass<UAbilitySystemComponent>();
		AbilitySystemComponent != nullptr)
	{
		return AbilitySystemComponent;
	}
	
	return TOptional<UAbilitySystemComponent*>();
}
