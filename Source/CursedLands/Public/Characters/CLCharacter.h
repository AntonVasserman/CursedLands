// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CLGameplayTags.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"
#include "CLCharacter.generated.h"

class UCLPawnData;
class UCLAbilitySystemComponent;
class UCLHealthAttributeSet;
class UGameplayEffect;

UCLASS()
class CURSEDLANDS_API ACLCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	ACLCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System")
	FORCEINLINE UCLAbilitySystemComponent* GetCLAbilitySystemComponent() const { return AbilitySystem; }
	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System|Attributes")
	FORCEINLINE UCLHealthAttributeSet* GetHealthAttributeSet() const { return HealthAttributeSet; }
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
	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability System")
	TObjectPtr<UCLAbilitySystemComponent> AbilitySystem;

	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability System|Attributes")
	TObjectPtr<UCLHealthAttributeSet> HealthAttributeSet;

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
