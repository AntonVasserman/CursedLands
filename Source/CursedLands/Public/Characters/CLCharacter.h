// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"
#include "CLCharacter.generated.h"

class UCLAttributeSet;
class UCLAbilitySystemComponent;
class UGameplayEffect;

UCLASS()
class CURSEDLANDS_API ACLCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	ACLCharacter();

	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System")
	FORCEINLINE UCLAbilitySystemComponent* GetCLAbilitySystemComponent() const { return AbilitySystem; }
	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System | Attributes")
	FORCEINLINE UCLAttributeSet* GetAttributeSet() const { return AttributeSet; }
	/**
	 * Tries to add a GameplayTag only if this Ability System Component doesn't already have it
	 * 
	 * @param GameplayTag to add
	 * 
	 * @return true if GameplayTag was added. Returns false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "GameplayTags")
	bool AddUniqueGameplayTag(const FGameplayTag& GameplayTag);
	UFUNCTION(BlueprintCallable, Category = "GameplayTags")
	void RemoveGameplayTag(const FGameplayTag& GameplayTag);
	
	void InitAbilityActorInfo();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Gameplay Ability System | Attributes | Effects")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributesEffectClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Gameplay Ability System | Attributes | Effects")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributesEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Gameplay Ability System | Attributes | Effects")
	TArray<TSubclassOf<UGameplayEffect>> DefaultPassiveEffectClasses;

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level);
	virtual void Die();
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Die")
	void Die_BP();
	void InitializeDefaultAttributes();
	void InitializeDefaultPassiveEffects();

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability System")
	TObjectPtr<UCLAbilitySystemComponent> AbilitySystem;

	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability System | Attributes")
	TObjectPtr<UCLAttributeSet> AttributeSet;
	
	//~ ACharacter Begin
public:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	//~ ACharacter End

	//~ IAbilitySystemInterface Begin
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ IAbilitySystemInterface End
	
	//~ IGameplayTagAssetInterface Begin
public:
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	//~ IGameplayTagAssetInterface End
};
