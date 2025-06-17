// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CLGameplayTags.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"
#include "CLCharacter.generated.h"

class UCL_AbilitySystemComponent;
class UCL_HealthComponent;
class UCLPawnData;
class UGameplayEffect;

UCLASS()
class CURSEDLANDS_API ACLCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability System")
	TObjectPtr<UCL_AbilitySystemComponent> AbilitySystem;

	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability System|Health")
	TObjectPtr<UCL_HealthComponent> HealthComponent;

public:
	ACLCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System")
	FORCEINLINE UCL_AbilitySystemComponent* GetCLAbilitySystemComponent() const { return AbilitySystem; }
	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System|Health")
	FORCEINLINE UCL_HealthComponent* GetHealthComponent() const { return HealthComponent; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsAlive() const { return HasMatchingGameplayTag(CLGameplayTags::Status_Alive); }
	FORCEINLINE bool CanMove() const { return IsAlive(); }
	void SimulatePhysics() const;

protected:
	// TODO: Should this be EditInstanceOnly? LyraGame uses EditInstanceOnly and set this either in a world object or using a Spawn function.
	UPROPERTY(EditDefaultsOnly, Category = "Config|Pawn")
	TObjectPtr<const UCLPawnData> PawnData;

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level);
	virtual void Die();
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Die")
	void Die_BP();
	UAnimInstance* GetAnimInstance();
	void SetMovementModeTag(const EMovementMode InMovementMode, const uint8 InCustomMovementMode, const bool bTagEnabled);

private:
	UFUNCTION()
	void OnHealthChanged(float OldValue, float NewValue);
	
	//~ ACharacter Begin
public:
	virtual void BeginPlay() override;
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void PostInitializeComponents() override;
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
