// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "CLAbilitySet.generated.h"

struct FGameplayAbilitySpecHandle;
struct FActiveGameplayEffectHandle;
class UGameplayEffect;
class UCLAbilitySystemComponent;
class UCLAttributeSet;
class UCLGameplayAbility;

/**
 *	Data used by the ability set to grant gameplay abilities.
 */
USTRUCT(BlueprintType)
struct FCLAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:
	// Gameplay ability to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCLGameplayAbility> Ability = nullptr;

	// Level of ability to grant.
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	// Tag used to process input for the ability.
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

/**
 *	Data used by the ability set to grant gameplay effects.
 */
USTRUCT(BlueprintType)
struct FCLAbilitySet_GameplayEffect
{
	GENERATED_BODY()
	
public:
	// Gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

	// Level of gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
	int32 EffectLevel = 1;
};

/**
 *	Data used by the ability set to grant attribute sets.
 */
USTRUCT(BlueprintType)
struct FCLAbilitySet_AttributeSet
{
	GENERATED_BODY()

public:
	// Gameplay attribute to grant
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCLAttributeSet> AttributeSet;
};

/**
 *	Data used to store handles to what has been granted by the ability set.
 */
USTRUCT(BlueprintType)
struct FCLAbilitySet_GrantedHandlers
{
	GENERATED_BODY()
	
public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddAttributeSet(UCLAttributeSet* Set);

	void RemoveFromAbilitySystem(UCLAbilitySystemComponent* ASC);

protected:
	// Handles to the granted abilities.
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	// Handles to the granted gameplay effects.
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	// Handles to the granted attribute sets.
	UPROPERTY()
	TArray<TObjectPtr<UCLAttributeSet>> AttributeSetHandles;
};

/**
 *	Non-mutable data asset used to grant gameplay abilities and gameplay effects.
 */
UCLASS(BlueprintType, Const)
class CURSEDLANDS_API UCLAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	void GiveToAbilitySystem(UCLAbilitySystemComponent* ASC, FCLAbilitySet_GrantedHandlers* OutGrantedHandlers, UObject* SourceObject = nullptr) const;

protected:
	// Gameplay abilities to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", Meta = (TitleProperty = Ability))
	TArray<FCLAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	// Gameplay effects to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", Meta = (TitleProperty = GameplayEffect))
	TArray<FCLAbilitySet_GameplayEffect> GrantedGameplayEffects;

	// Attribute sets to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets", Meta = (TitleProperty = AttributeSet))
	TArray<FCLAbilitySet_AttributeSet> GrantedAttributes;

private:
	void GiveAttributeSetsToAbilitySystem(UCLAbilitySystemComponent* ASC, FCLAbilitySet_GrantedHandlers* OutGrantedHandlers) const;
	void GiveGameplayAbilitiesToAbilitySystem(UCLAbilitySystemComponent* ASC, FCLAbilitySet_GrantedHandlers* OutGrantedHandlers, UObject* SourceObject = nullptr) const;
	void GiveGameplayEffectsToAbilitySystem(UCLAbilitySystemComponent* ASC, FCLAbilitySet_GrantedHandlers* OutGrantedHandlers) const;
};
