// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLGameplayTags.h"
#include "CLPlayerCharacterCameraMode.h"
#include "CLPlayerCharacterMovementMode.h"
#include "MotionWarpingComponent.h"
#include "AbilitySystem/CL_AbilitySystemComponent.h"
#include "Characters/CLCharacter.h"
#include "Components/CLCharacterMovementComponent.h"
#include "TraversalSystem/CLCharacterTraversalComponent.h"
#include "CLPlayerCharacter.generated.h"

enum class ECLPlayerCharacterCameraMode : uint8;
class UCLCharacterFallingComponent;
class UCLCharacterTraversalComponent;
class UCL_ManaComponent;
class UCL_StaminaComponent;
class UGameplayCameraComponent;

CURSEDLANDS_API DECLARE_LOG_CATEGORY_EXTERN(LogCLPlayerCharacter, Log, All);

UENUM(BlueprintType)
enum class ECLCardinalDirection : uint8
{
	Forward		UMETA(DisplayName = "Forward"),
	Backward	UMETA(DisplayName = "Backward"),
	Right		UMETA(DisplayName = "Right"),
	Left		UMETA(DisplayName = "Left"),
};

UCLASS()
class CURSEDLANDS_API ACLPlayerCharacter : public ACLCharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = "Locomotion|Falling")
	TObjectPtr<UCLCharacterFallingComponent> FallingComponent;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability System|Mana")
	TObjectPtr<UCL_ManaComponent> ManaComponent;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability System|Stamina")
	TObjectPtr<UCL_StaminaComponent> StaminaComponent;

public:
	ACLPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	FORCEINLINE UGameplayCameraComponent* GetGameplayCamera() const { return GameplayCamera; }
	UFUNCTION(BlueprintCallable, Category = "Gameplay Camera System")
	FORCEINLINE ECLPlayerCharacterCameraMode GetCameraMode() const { return CameraMode; }
	UFUNCTION(BlueprintCallable, Category = "Gameplay Camera System")
	FORCEINLINE void SetCameraMode(const ECLPlayerCharacterCameraMode InCameraMode) { CameraMode = InCameraMode; }
	
	UFUNCTION(BlueprintCallable, Category = "Locomotion")
	FORCEINLINE ECLPlayerCharacterMovementMode GetMovementMode() const { return MovementMode; }
	UFUNCTION(BlueprintCallable, Category = "Locomotion")
	void SetMovementMode(const ECLPlayerCharacterMovementMode InMovementMode);
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System|Stamina")
	FORCEINLINE UCL_StaminaComponent* GetStaminaComponent() const { return StaminaComponent; }
	FORCEINLINE bool CanLook() const { return IsAlive(); }
	
	UFUNCTION(BlueprintCallable, Category = "Character Movement")
	FORCEINLINE UCLCharacterMovementComponent* GetCLCharacterMovement() const { return CastChecked<UCLCharacterMovementComponent>(GetCharacterMovement()); }

	UFUNCTION(BlueprintCallable, Category = "Character Movement|Stance|Stand")
	FORCEINLINE bool IsStanding() const { return HasMatchingGameplayTag(CLGameplayTags::Locomotion_Stance_Standing); }
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Stance|Crouch")
	FORCEINLINE bool IsCrouching() const { return HasMatchingGameplayTag(CLGameplayTags::Locomotion_Stance_Crouching); }
	
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Walking|Walk")
	bool CanWalk() const;
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Walking|Walk")
	FORCEINLINE bool IsWalking() const { return HasMatchingGameplayTag(CLGameplayTags::Locomotion_Gait_Walking); }
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Walking|Walk")
	void Walk();
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Walking|Walk")
	void UnWalk();

	UFUNCTION(BlueprintCallable, Category = "Character Movement|Walking|Sprint")
	bool CanSprint() const;
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Walking|Sprint")
	FORCEINLINE bool IsSprinting() const { return HasMatchingGameplayTag(CLGameplayTags::Locomotion_Gait_Sprinting); }
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Walking|Sprint")
	void Sprint();
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Walking|Sprint")
	void UnSprint();

	UFUNCTION(BlueprintCallable, Category = "Locomotion|Falling")
	FORCEINLINE UCLCharacterFallingComponent* GetFallingComponent() const { return FallingComponent; }
	// TODO (CL-156): Remove in favor of additional case in CharacterFallingComponent
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Falling")
	FORCEINLINE float GetFallHeightForMinFallDamage() const { return FallHeightForMinFallDamage; }
	// TODO (CL-156): Remove in favor of additional case in CharacterFallingComponent
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Falling")
	FORCEINLINE float GetFallHeightForMaxFallDamage() const { return FallHeightForMaxFallDamage; }

	UFUNCTION(BlueprintCallable, Category = "Character Traversal")
	FORCEINLINE UCLCharacterTraversalComponent* GetCharacterTraversal() const { return CharacterTraversal; }
	UFUNCTION(BlueprintCallable, Category = "Character Traversal")
	FORCEINLINE bool IsTraversing() const { return CharacterTraversal->IsDoingTraversalAction(); }
	UFUNCTION(BlueprintCallable, Category = "Character Traversal")
	bool CanTraverse() const;
	void Traverse();

	UFUNCTION(BlueprintCallable, Category = "Character Traversal")
	bool CanSlide() const;
	void Slide();
	
	UFUNCTION(BlueprintCallable, Category = "Locomotion")
	FORCEINLINE float GetCardinalDirectionAngle() const { return CardinalDirectionAngle; }
	UFUNCTION(BlueprintCallable, Category = "Locomotion")
	FORCEINLINE ECLCardinalDirection GetCardinalDirection() const { return CardinalDirection; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay Camera System", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGameplayCameraComponent> GameplayCamera;
	FVector InitialGameplayCameraRelativeLocation = FVector(0.f, 0.f, 100.f);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion Warping", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMotionWarpingComponent> MotionWarping;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Traversal", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCLCharacterTraversalComponent> CharacterTraversal;

	ECLPlayerCharacterCameraMode CameraMode = ECLPlayerCharacterCameraMode::Default;
	ECLPlayerCharacterMovementMode MovementMode = ECLPlayerCharacterMovementMode::Default;

	// TODO (CL-156): Remove in favor of additional case in CharacterFallingComponent
	UPROPERTY(EditDefaultsOnly, Category = "Config|Character Movement|Falling", Meta = (AllowPrivateAccess = "true"))
	float FallHeightForMinFallDamage = 800.f;

	// TODO (CL-156): Remove in favor of additional case in CharacterFallingComponent
	UPROPERTY(EditDefaultsOnly, Category = "Config|Character Movement|Falling", Meta = (AllowPrivateAccess = "true"))
	float FallHeightForMaxFallDamage = 1500.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config|Character Movement|Walking", Meta = (AllowPrivateAccess = "true"))
	float MinWalkSpeed = 20.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config|Character Movement|Gameplay Ability System", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> FatigueGameplayEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Config|Locomotion", Meta = (AllowPrivateAccess = "true"))
	float CardinalDirectionBackwardMin = -130.f;
	UPROPERTY(EditDefaultsOnly, Category = "Config|Locomotion", Meta = (AllowPrivateAccess = "true"))
	float CardinalDirectionBackwardMax = 130.f;
	UPROPERTY(EditDefaultsOnly, Category = "Config|Locomotion", Meta = (AllowPrivateAccess = "true"))
	float CardinalDirectionForwardMin = -50.f;
	UPROPERTY(EditDefaultsOnly, Category = "Config|Locomotion", Meta = (AllowPrivateAccess = "true"))
	float CardinalDirectionForwardMax = 50.f;
	UPROPERTY(EditDefaultsOnly, Category = "Config|Locomotion", Meta = (AllowPrivateAccess = "true"))
	float CardinalDirectionDeadzone = 20.f;

	float CardinalDirectionAngle = 0.f;
	ECLCardinalDirection CardinalDirection = ECLCardinalDirection::Forward;
	bool bFullySprinting = false;
	bool bSprintAfterTraversal = false;

	void ApplyFatigue();
	UFUNCTION()
	void OnStanceChanged(ECLStance PreviousStance, ECLStance Stance);
	UFUNCTION()
	void OnGaitChanged(ECLGait PreviousGait, ECLGait Gait);
	UFUNCTION()
	void OnStaminaChanged(float OldValue, float NewValue);
	void SetStanceTag(const ECLStance InStance, const bool bTagEnabled) const;
	void SetGaitTag(const ECLGait InGait, const bool bTagEnabled) const;
	void SetTraversalActionTag(const ECLTraversalAction InTraversalAction, const bool bTagEnabled) const;
	void UpdateCardinalDirectionAngle();
	void UpdateCardinalDirection();

	// CharacterTraversalComponent Bindings
	UFUNCTION()
	void OnCharacterTraversalActionStarted(const ECLTraversalAction TraversalAction);
	UFUNCTION()
	void OnCharacterTraversalActionFinished(const ECLTraversalAction TraversalAction);

	//~ ACLCharacter Begin
public:
	virtual void BeginPlay() override;
	virtual void Crouch(bool bClientSimulation = false) override;
	virtual bool CanCrouch() const override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void Die() override;
	//~ ACLCharacter End
	
};
