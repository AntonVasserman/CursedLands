// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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
	ACLPlayerCharacter();
	
	FORCEINLINE UGameplayCameraComponent* GetGameplayCamera() const { return GameplayCamera; }
	UFUNCTION(BlueprintCallable, Category = "Gameplay Camera System")
	FORCEINLINE ECLPlayerCharacterCameraMode GetCameraMode() const { return CameraMode; }
	UFUNCTION(BlueprintCallable, Category = "Gameplay Camera System")
	FORCEINLINE void SetCameraMode(const ECLPlayerCharacterCameraMode InCameraMode) { CameraMode = InCameraMode; }
	UFUNCTION(BlueprintCallable, Category = "Locomotion")
	FORCEINLINE ECLPlayerCharacterMovementMode GetMovementMode() const { return MovementMode; }
	UFUNCTION(BlueprintCallable, Category = "Locomotion")
	void SetMovementMode(const ECLPlayerCharacterMovementMode InMovementMode);
	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System | Attributes")
	FORCEINLINE UCLStaminaAttributeSet* GetStaminaAttributeSet() const { return StaminaAttributeSet; }

	FORCEINLINE bool CanLook() const { return IsAlive(); }
	bool CanSprint() const;
	UFUNCTION(BlueprintCallable, Category = "Character Locomotion")
	FORCEINLINE bool IsSprinting() const { return GetAbilitySystemComponent()->HasMatchingGameplayTag(FCLGameplayTags::Get().Locomotion_Sprinting); }
	void ToggleSprint();
	void UnToggleSprint();
	UFUNCTION(BlueprintCallable, Category = "Character Locomotion")
	FORCEINLINE float GetFallHeightForMinFallDamage() const { return FallHeightForMinFallDamage; }
	UFUNCTION(BlueprintCallable, Category = "Character Locomotion")
	FORCEINLINE float GetFallHeightForMaxFallDamage() const { return FallHeightForMaxFallDamage; }
	UFUNCTION(BlueprintCallable, Category = "Character Locomotion")
	FORCEINLINE float GetFallHeight() const { return FallHeight; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay Camera System", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGameplayCameraComponent> GameplayCamera;

	ECLPlayerCharacterCameraMode CameraMode = ECLPlayerCharacterCameraMode::Default;
	ECLPlayerCharacterMovementMode MovementMode = ECLPlayerCharacterMovementMode::Default;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Character Locomotion", Meta = (AllowPrivateAccess = "true"))
	float FallHeightForMinFallDamage = 800.f;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Character Locomotion", Meta = (AllowPrivateAccess = "true"))
	float FallHeightForMaxFallDamage = 1500.f;

	float FallHeight = 0.f;
	float FallBeginZ = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Character Locomotion | Animation", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> FallToRollAnimMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config | Character Locomotion | Gameplay Ability System", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> FallDamageGameplayEffectClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config | Character Locomotion", Meta = (AllowPrivateAccess = "true"))
	float MinWalkSpeed = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Character Locomotion", Meta = (AllowPrivateAccess = "true"))
	float RunSpeed = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Character Locomotion", Meta = (AllowPrivateAccess = "true"))
	float SprintSpeed = 900.f;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Character Locomotion | Gameplay Ability System", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> FatigueGameplayEffectClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability System | Attributes")
	TObjectPtr<UCLManaAttributeSet> ManaAttributeSet;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability System | Attributes")
	TObjectPtr<UCLStaminaAttributeSet> StaminaAttributeSet;

	void ApplyFatigue();
	void PlayFallToRollAnimMontage();

	//~ ACLCharacter Begin
public:
	virtual void BeginPlay() override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;
	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void Die() override;
	//~ ACLCharacter End
	
};
