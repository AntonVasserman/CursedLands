// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "CL_AbilitySet.generated.h"

struct FActiveGameplayEffectHandle;
struct FGameplayAbilitySpecHandle;
class UCL_AbilitySystemComponent;
class UCL_AttributeSet;
class UCL_GameplayAbility;
class UGameplayEffect;

/**
 *	Data used by the ability set to grant gameplay abilities.
 */
USTRUCT(BlueprintType)
struct FCL_AbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:
	// Gameplay ability to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCL_GameplayAbility> Ability = nullptr;

	// Level of ability to grant.
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	// Tag used to process input for the ability.
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag.Ability"))
	FGameplayTag InputTag;
};

/**
 *	Data used by the ability set to grant gameplay effects.
 */
USTRUCT(BlueprintType)
struct FCL_AbilitySet_GameplayEffect
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
struct FCL_AbilitySet_AttributeSet
{
	GENERATED_BODY()

public:
	// Gameplay attribute to grant
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCL_AttributeSet> AttributeSet;
};

/**
 *	Data used to store handles to what has been granted by the ability set.
 */
USTRUCT(BlueprintType)
struct FCL_AbilitySet_GrantedHandlers
{
	GENERATED_BODY()
	
public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddAttributeSet(UCL_AttributeSet* Set);

	void RemoveFromAbilitySystem(UCL_AbilitySystemComponent* ASC);

protected:
	// Handles to the granted abilities.
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	// Handles to the granted gameplay effects.
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	// Handles to the granted attribute sets.
	UPROPERTY()
	TArray<TObjectPtr<UCL_AttributeSet>> AttributeSetHandles;
};

/**
 *	Non-mutable data asset used to grant gameplay abilities and gameplay effects.
 */
UCLASS(BlueprintType, Const)
class CURSEDLANDS_API UCL_AbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	void GiveToAbilitySystem(UCL_AbilitySystemComponent* ASC, FCL_AbilitySet_GrantedHandlers* OutGrantedHandlers, UObject* SourceObject = nullptr) const;

protected:
	// Gameplay abilities to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", Meta = (TitleProperty = Ability))
	TArray<FCL_AbilitySet_GameplayAbility> GrantedGameplayAbilities;

	// Gameplay effects to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", Meta = (TitleProperty = GameplayEffect))
	TArray<FCL_AbilitySet_GameplayEffect> GrantedGameplayEffects;

	// Attribute sets to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets", Meta = (TitleProperty = AttributeSet))
	TArray<FCL_AbilitySet_AttributeSet> GrantedAttributes;

private:
	void GiveAttributeSetsToAbilitySystem(UCL_AbilitySystemComponent* ASC, FCL_AbilitySet_GrantedHandlers* OutGrantedHandlers) const;
	void GiveGameplayAbilitiesToAbilitySystem(UCL_AbilitySystemComponent* ASC, FCL_AbilitySet_GrantedHandlers* OutGrantedHandlers, UObject* SourceObject = nullptr) const;
	void GiveGameplayEffectsToAbilitySystem(UCL_AbilitySystemComponent* ASC, FCL_AbilitySet_GrantedHandlers* OutGrantedHandlers) const;
};
