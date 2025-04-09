// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"
#include "CLCharacter.generated.h"

class UCLAbilitySystemComponent;
class UCLHealthAttributeSet;
class UGameplayEffect;

UCLASS()
class CURSEDLANDS_API ACLCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	ACLCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System")
	FORCEINLINE UCLAbilitySystemComponent* GetCLAbilitySystemComponent() const { return AbilitySystem; }
	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System | Attributes")
	FORCEINLINE UCLHealthAttributeSet* GetHealthAttributeSet() const { return HealthAttributeSet; }
	FORCEINLINE bool CanMove() const { return IsAlive(); }
	FORCEINLINE bool IsAlive() const { return bIsAlive; }
	void SimulatePhysics() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Gameplay Ability System | Attributes | Effects")
	TSubclassOf<UGameplayEffect> DefaultGeneralAttributesValueEffectClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Gameplay Ability System | Attributes | Effects")
	TSubclassOf<UGameplayEffect> DefaultGeneralAttributesMaxValueEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Gameplay Ability System | Attributes | Effects")
	TArray<TSubclassOf<UGameplayEffect>> DefaultPassiveEffectClasses;

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level);
	virtual void Die();
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Die")
	void Die_BP();
	UAnimInstance* GetAnimInstance();
	void InitializeDefaultAttributes();
	void InitializeDefaultPassiveEffects();
	void SetMovementModeTag(EMovementMode MovementMode, uint8 CustomMovementMode, bool bTagEnabled);

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability System")
	TObjectPtr<UCLAbilitySystemComponent> AbilitySystem;

	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability System | Attributes")
	TObjectPtr<UCLHealthAttributeSet> HealthAttributeSet;

	bool bIsAlive = true;
	
	//~ ACharacter Begin
public:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void PostInitializeComponents() override;
	//~ ACharacter End

	//~ IAbilitySystemInterface Begin
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
