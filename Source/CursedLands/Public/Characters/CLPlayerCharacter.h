// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLCharacterMovementComponent.h"
#include "CLGameplayTags.h"
#include "CLPlayerCharacterCameraMode.h"
#include "CLPlayerCharacterMovementMode.h"
#include "AbilitySystem/CLAbilitySystemComponent.h"
#include "Characters/CLCharacter.h"
#include "CLPlayerCharacter.generated.h"

enum class ECLPlayerCharacterCameraMode : uint8;
class UCLManaAttributeSet;
class UCLStaminaAttributeSet;
class UGameplayCameraComponent;

UCLASS()
class CURSEDLANDS_API ACLPlayerCharacter : public ACLCharacter
{
	GENERATED_BODY()

public:
	ACLPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// TODO (202504-2): Design a solution where this isn't public and isn't set by the CharacterMovementComponent directly...
	// This can be achieved with two Events in the CLCMC, OnStartSprint and OnEdnSprint, or just when WalkingChanged.
	// We also can and should consider not having this bool, but just check the tag, since our character is integrated
	// with Tags and should lean on them as the source of truth.
	UPROPERTY(BlueprintReadOnly, Category = "Character Movement|Walking|Sprinting")
	uint8 bIsSprinting:1 {false};
	
	FORCEINLINE UGameplayCameraComponent* GetGameplayCamera() const { return GameplayCamera; }
	UFUNCTION(BlueprintCallable, Category = "Gameplay Camera System")
	FORCEINLINE ECLPlayerCharacterCameraMode GetCameraMode() const { return CameraMode; }
	UFUNCTION(BlueprintCallable, Category = "Gameplay Camera System")
	FORCEINLINE void SetCameraMode(const ECLPlayerCharacterCameraMode InCameraMode) { CameraMode = InCameraMode; }
	
	UFUNCTION(BlueprintCallable, Category = "Locomotion")
	FORCEINLINE ECLPlayerCharacterMovementMode GetMovementMode() const { return MovementMode; }
	UFUNCTION(BlueprintCallable, Category = "Locomotion")
	void SetMovementMode(const ECLPlayerCharacterMovementMode InMovementMode);
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System|Attributes")
	FORCEINLINE UCLStaminaAttributeSet* GetStaminaAttributeSet() const { return StaminaAttributeSet; }
	FORCEINLINE bool CanLook() const { return IsAlive(); }
	
	UFUNCTION(BlueprintCallable, Category = "Character Movement")
	FORCEINLINE UCLCharacterMovementComponent* GetCLCharacterMovement() const { return CastChecked<UCLCharacterMovementComponent>(GetCharacterMovement()); }
	
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Walking|Sprint")
	bool CanSprint() const;
	void Sprint();
	void UnSprint();
	
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Falling")
	FORCEINLINE float GetFallHeightForMinFallDamage() const { return FallHeightForMinFallDamage; }
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Falling")
	FORCEINLINE float GetFallHeightForMaxFallDamage() const { return FallHeightForMaxFallDamage; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay Camera System", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGameplayCameraComponent> GameplayCamera;

	ECLPlayerCharacterCameraMode CameraMode = ECLPlayerCharacterCameraMode::Default;
	ECLPlayerCharacterMovementMode MovementMode = ECLPlayerCharacterMovementMode::Default;

	UPROPERTY(EditDefaultsOnly, Category = "Config|Character Movement|Falling", Meta = (AllowPrivateAccess = "true"))
	float FallHeightForMinFallDamage = 800.f;

	UPROPERTY(EditDefaultsOnly, Category = "Config|Character Movement|Falling", Meta = (AllowPrivateAccess = "true"))
	float FallHeightForMaxFallDamage = 1500.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config|Character Movement|Falling|Animation", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> FallToRollAnimMontage;
	FOnMontageEnded FallToRollAnimMontageEndedDelegate;

	UPROPERTY(EditDefaultsOnly, Category = "Config|Character Movement|Falling|Animation", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> FallToDeathAnimMontage;
	const FName FallToDeathAnimMontage_SectionName_Impact = FName("Impact");
	const FName FallToDeathAnimMontage_SectionName_DeathLoop = FName("DeathLoop");
	
	UPROPERTY(EditDefaultsOnly, Category = "Config|Character Movement|Falling|Gameplay Ability System", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> FallDamageGameplayEffectClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config|Character Movement|Walking", Meta = (AllowPrivateAccess = "true"))
	float MinWalkSpeed = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Config|Character Movement|Gameplay Ability System", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> FatigueGameplayEffectClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability System|Attributes")
	TObjectPtr<UCLManaAttributeSet> ManaAttributeSet;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability System|Attributes")
	TObjectPtr<UCLStaminaAttributeSet> StaminaAttributeSet;

	void ApplyFatigue();
	UFUNCTION()
	void OnMovementWalkingModeChanged(ECLMovementWalkingMode PreviousMovementWalkingMode, ECLMovementWalkingMode MovementWalkingMode);
	void PlayFallToRollAnimMontage();
	void PlayFallToDeathAnimMontage();

	//~ ACLCharacter Begin
public:
	virtual void Landed(const FHitResult& Hit) override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void Die() override;
	//~ ACLCharacter End
	
};
