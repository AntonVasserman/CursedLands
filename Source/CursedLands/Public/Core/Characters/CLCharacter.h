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
	
	FORCEINLINE UCLAbilitySystemComponent* GetAbilitySystem() const { return AbilitySystem; }
	FORCEINLINE UCLAttributeSet* GetAttributeSet() const { return AttributeSet; }

	void InitAbilityActorInfo();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System | Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System | Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level) const;
	void InitializeDefaultAttributes() const;

private:
	UPROPERTY()
	TObjectPtr<UCLAbilitySystemComponent> AbilitySystem;

	UPROPERTY()
	TObjectPtr<UCLAttributeSet> AttributeSet;

	//~ ACharacter Begin
	
public:
	virtual void PossessedBy(AController* NewController) override;
	
	//~ ACharacter End
};
