// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_GameplayTags.h"
#include "CL_PlayerCharacterCameraMode.h"
#include "CL_PlayerCharacterMovementMode.h"
#include "MotionWarpingComponent.h"
#include "Characters/CL_Character.h"
#include "Components/CL_ExtendedCharacterMovementComponent.h"
#include "Systems/Traversal/CL_CharacterTraversalComponent.h"
#include "CL_PlayerCharacter.generated.h"

enum class ECL_PlayerCharacterCameraMode : uint8;
class UAV_CharacterFallComponent;
class UCL_CharacterFallingComponent;
class UCL_CharacterTraversalComponent;
class UCL_ManaComponent;
class UCL_StaminaComponent;
class UGameplayCameraComponent;

CURSEDLANDS_API DECLARE_LOG_CATEGORY_EXTERN(LogCLPlayerCharacter, Log, All);

UENUM(BlueprintType)
enum class ECL_CardinalDirection : uint8
{
	Forward		UMETA(DisplayName = "Forward"),
	Backward	UMETA(DisplayName = "Backward"),
	Right		UMETA(DisplayName = "Right"),
	Left		UMETA(DisplayName = "Left"),
};

UCLASS()
class CURSEDLANDS_API ACL_PlayerCharacter : public ACL_Character
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ConcreteMesh;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Character Fall")
	TObjectPtr<UAV_CharacterFallComponent> CharacterFallComponent;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability System|Mana")
	TObjectPtr<UCL_ManaComponent> ManaComponent;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability System|Stamina")
	TObjectPtr<UCL_StaminaComponent> StaminaComponent;

public:
	ACL_PlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	FORCEINLINE UGameplayCameraComponent* GetGameplayCamera() const { return GameplayCamera; }
	UFUNCTION(BlueprintCallable, Category = "Gameplay Camera System")
	FORCEINLINE ECL_PlayerCharacterCameraMode GetCameraMode() const { return CameraMode; }
	UFUNCTION(BlueprintCallable, Category = "Gameplay Camera System")
	FORCEINLINE void SetCameraMode(const ECL_PlayerCharacterCameraMode InCameraMode) { CameraMode = InCameraMode; }
	
	UFUNCTION(BlueprintCallable, Category = "Locomotion")
	FORCEINLINE ECL_PlayerCharacterMovementMode GetMovementMode() const { return MovementMode; }
	UFUNCTION(BlueprintCallable, Category = "Locomotion")
	void SetMovementMode(const ECL_PlayerCharacterMovementMode InMovementMode);
	UFUNCTION(BlueprintCallable, Category = "Locomotion")
	FORCEINLINE bool IsRolling() const { return HasMatchingGameplayTag(CLGameplayTags::Locomotion_Rolling); }
	UFUNCTION(BlueprintCallable, Category = "Locomotion")
	FORCEINLINE bool IsStandingUp() const { return HasMatchingGameplayTag(CLGameplayTags::Locomotion_StandingUp); }

	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System|Mana")
	FORCEINLINE UCL_ManaComponent* GetManaComponent() const { return ManaComponent; }
	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System|Stamina")
	FORCEINLINE UCL_StaminaComponent* GetStaminaComponent() const { return StaminaComponent; }
	FORCEINLINE bool CanLook() const { return IsAlive(); }
	
	UFUNCTION(BlueprintCallable, Category = "Character Movement")
	FORCEINLINE UCL_ExtendedCharacterMovementComponent* GetCLCharacterMovement() const { return CastChecked<UCL_ExtendedCharacterMovementComponent>(GetCharacterMovement()); }

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

	UFUNCTION(BlueprintCallable, Category = "Character Fall")
	FORCEINLINE UAV_CharacterFallComponent* GetCharacterFallComponent() const { return CharacterFallComponent; }
	UFUNCTION(BlueprintCallable, Category = "Fall")
	ECL_FallType GetFallType() const;
	
	UFUNCTION(BlueprintCallable, Category = "Character Traversal")
	FORCEINLINE UCL_CharacterTraversalComponent* GetCharacterTraversal() const { return CharacterTraversal; }
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
	FORCEINLINE ECL_CardinalDirection GetCardinalDirection() const { return CardinalDirection; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay Camera System", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGameplayCameraComponent> GameplayCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion Warping", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMotionWarpingComponent> MotionWarping;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Traversal", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCL_CharacterTraversalComponent> CharacterTraversal;

	ECL_PlayerCharacterCameraMode CameraMode = ECL_PlayerCharacterCameraMode::Default;
	ECL_PlayerCharacterMovementMode MovementMode = ECL_PlayerCharacterMovementMode::Default;

	UPROPERTY(EditDefaultsOnly, Category = "Config|Character Movement|Walking", Meta = (AllowPrivateAccess = "true"))
	float MinWalkSpeed = 20.f;

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

	UPROPERTY(BlueprintReadonly, Meta = (AllowPrivateAccess = "true"))
	bool bJustLanded = false;
	
	float CardinalDirectionAngle = 0.f;
	ECL_CardinalDirection CardinalDirection = ECL_CardinalDirection::Forward;
	float JustLandedResetTime = 0.f;
	bool bFullySprinting = false;
	bool bSprintAfterTraversal = false;
	
	void FatigueApplied();
	UFUNCTION()
	void OnStanceChanged(ECL_Stance PreviousStance, ECL_Stance Stance);
	UFUNCTION()
	void OnGaitChanged(ECL_Gait PreviousGait, ECL_Gait Gait);
	void SetStanceTag(const ECL_Stance InStance, const bool bTagEnabled) const;
	void SetGaitTag(const ECL_Gait InGait, const bool bTagEnabled) const;
	void SetTraversalActionTag(const ECL_TraversalAction InTraversalAction, const bool bTagEnabled) const;
	void UpdateCardinalDirectionAngle();
	void UpdateCardinalDirection();

	// CharacterTraversalComponent Bindings
	UFUNCTION()
	void OnCharacterTraversalActionStarted(const ECL_TraversalAction TraversalAction);
	UFUNCTION()
	void OnCharacterTraversalActionFinished(const ECL_TraversalAction TraversalAction);

	//~ ACLCharacter Begin
public:
	virtual void BeginPlay() override;
	virtual void Crouch(bool bClientSimulation = false) override;
	virtual bool CanCrouch() const override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void Die() override;
	virtual void OnGameplayTagNewOrRemoved(FGameplayTag GameplayTag, int NewCount) override;
	//~ ACLCharacter End
};
