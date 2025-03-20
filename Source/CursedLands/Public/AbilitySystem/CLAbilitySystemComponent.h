// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CLAbilitySystemComponent.generated.h"

struct FCLGameplayTags;

UCLASS()
class CURSEDLANDS_API UCLAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	/**
	 * Tries to add a GameplayTag only if this Ability System Component doesn't already have it
	 * 
	 * @param GameplayTag to add
	 * 
	 * @return true if GameplayTag was added. Returns false otherwise.
	 */
	bool AddUniqueGameplayTag(const FGameplayTag& GameplayTag);

	/**
	 * Tries to remove a GameplayTag only if this Ability System Component has it
	 * 
	 * @param GameplayTag to remove
	 * 
	* @return true if GameplayTag was removed. Returns false otherwise.
	 */
	bool RemoveGameplayTag(const FGameplayTag& GameplayTag);
	
	//~ UAbilitySystemComponent Begin
	//~ UAbilitySystemComponent End
};
