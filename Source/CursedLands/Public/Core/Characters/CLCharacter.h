// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CLCharacter.generated.h"

class UCLAttributeSet;
class UCLAbilitySystemComponent;
class UGameplayEffect;

UCLASS()
class CURSEDLANDS_API ACLCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACLCharacter();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCLAbilitySystemComponent* GetAbilitySystem() const { return AbilitySystem; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCLAttributeSet* GetAttributeSet() const { return AttributeSet; }

	void InitAbilityActorInfo();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Gameplay Ability System | Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributesEffectClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Gameplay Ability System | Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributesEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Gameplay Ability System | Attributes")
	TArray<TSubclassOf<UGameplayEffect>> DefaultPassiveEffectClasses;

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level);
	virtual void Die();
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Die")
	void Die_BP();
	void InitializeDefaultAttributes();
	void InitializeDefaultPassiveEffects();

private:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UCLAbilitySystemComponent> AbilitySystem;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UCLAttributeSet> AttributeSet;
	
	//~ ACharacter Begin
public:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	//~ ACharacter End
};
