// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CLCharacter.generated.h"

class UCLAttributeSet;
class UCLAbilitySystemComponent;
class UGameplayEffect;
class UGameplayCameraComponent;

UCLASS()
class CURSEDLANDS_API ACLCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACLCharacter();

	FORCEINLINE UGameplayCameraComponent* GetGameplayCamera() const { return GameplayCamera; }
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGameplayCameraComponent> GameplayCamera;

	UPROPERTY()
	TObjectPtr<UCLAbilitySystemComponent> AbilitySystem;

	UPROPERTY()
	TObjectPtr<UCLAttributeSet> AttributeSet;

	//~ ACharacter Begin

public:
	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	//~ ACharacter End
};
