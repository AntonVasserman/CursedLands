// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Compute/AgentMessage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CLCharacterMovementComponent.generated.h"

class ACLPlayerCharacter;

UENUM(BlueprintType)
enum ECLCustomMovementMode : uint8
{
	CMOVE_None		UMETA(DisplayName = "None"),
	// Placeholder for Custom Movement Modes
};

UENUM(BlueprintType)
enum class ECLStance : uint8
{
	Standing	UMETA(DisplayName = "Standing"),
	Crouching	UMETA(DisplayName = "Crouching"),
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStanceChanged, ECLStance, PreviousStance, ECLStance, Stance);

UENUM(BlueprintType)
enum class ECLGait : uint8
{
	Walking		UMETA(DisplayName = "Walking"),
	Jogging		UMETA(DisplayName = "Jogging"),
	Sprinting	UMETA(DisplayName = "Sprinting"),
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGaitChanged, ECLGait, PreviousGait, ECLGait, Gait);

USTRUCT(BlueprintType)
struct FCLCharacterMovementProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Properties|Walking|Walk")
	uint8 bCanEverWalk:1 = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Properties|Walking|Sprint")
	uint8 bCanEverSprint:1 = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Properties|Walking|Sprint")
	uint8 bStopSprintingOnLanding:1 = false;
};

USTRUCT(BlueprintType)
struct FCLGaitSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float MaxWalkingSpeed = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float MaxAcceleration = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float BrakingDeceleration = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float BrakingFrictionFactor = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float BrakingFriction = 0.f;

	UPROPERTY(EditDefaultsOnly)
	uint8 bUseSeparateBrakingFriction:1 = false;
};

UCLASS(BlueprintType, Const)
class CURSEDLANDS_API UCLGaitSettingsData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	FCLGaitSettings GaitSettings;
};

UENUM(BlueprintType)
enum class ECLGaitSettingsNotFoundHandlingOption : uint8
{
	UseDefault	UMETA(DisplayName = "Use Default", Description = "In case Gait Settings not found fallback to Default Gait Settings. Make sure Default Gait Settings are set."),
	Halt		UMETA(DisplayName = "Halt", Description = "In case Gait Settings not found execute check assertion to halt the engine."),
};

USTRUCT(BlueprintType)
struct FCLGaitSettingsCollection
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, DisplayName = "Settings Mapping")
	TMap<ECLGait, TObjectPtr<const UCLGaitSettingsData>> SettingsMap;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Settings Not Found Handling Option")
	ECLGaitSettingsNotFoundHandlingOption SettingsNotFoundHandlingOption = ECLGaitSettingsNotFoundHandlingOption::UseDefault;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Default Settings in case of Not Found", meta = (EditCondition = "SettingsNotFoundHandlingOption == ECLGaitSettingsNotFoundHandlingOption::UseDefault", EditConditionHides))
	TObjectPtr<const UCLGaitSettingsData> DefaultSettings;
	
	const FCLGaitSettings& GetSettingsForGait(const ECLGait InGait) const
	{
		if (SettingsMap.Contains(InGait))
		{
			return SettingsMap[InGait]->GaitSettings;
		}

		switch (SettingsNotFoundHandlingOption)
		{
		case ECLGaitSettingsNotFoundHandlingOption::UseDefault:
			return DefaultSettings->GaitSettings;
		case ECLGaitSettingsNotFoundHandlingOption::Halt:
		default:
			checkNoEntry();
			break;
		}

		// We should never reach this point.
		return DefaultSettings->GaitSettings;
	}
};

UCLASS()
class CURSEDLANDS_API UCLCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	FOnStanceChanged OnStanceChanged;
	FOnGaitChanged OnGaitChanged;

	FORCEINLINE ECLStance GetStance() const { return Stance; }
	FORCEINLINE ECLGait GetGait() const { return Gait; }
	FCLGaitSettings GetGaitSettings(const ECLGait InGait) const;

	UFUNCTION(BlueprintCallable, Category = "Character Movement|Walking|Walk")
	FORCEINLINE bool CanEverWalk() const { return CharacterMovementProps.bCanEverWalk; }
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Walking|Walk")
	FORCEINLINE bool CanWalkInCurrentState() const { return CanEverWalk() && IsMovingOnGround(); }
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Walking|Walk")
	void RequestWalking();
	
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Walking|Jog")
	void RequestJogging();
	
	FORCEINLINE bool CanEverSprint() const { return CharacterMovementProps.bCanEverSprint; }
	FORCEINLINE bool CanSprintInCurrentState() const { return CanEverSprint() && !Velocity.IsNearlyZero() && IsMovingOnGround(); }
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Walking|Sprint")
	void RequestSprinting();
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Walking|Sprint")
	void RequestUnSprinting();
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Walking|Sprint")
	void Sprint();
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Walking|Sprint")
	void UnSprint();
	
private:
	uint8 bWantsToSprint:1 = false;
	
	UPROPERTY(Transient, DuplicateTransient)
	TObjectPtr<ACLPlayerCharacter> PlayerCharacterOwner;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion", Meta = (AllowPrivateAccess))
	ECLStance Stance = ECLStance::Standing;
	
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion", Meta = (AllowPrivateAccess))
	ECLGait Gait = ECLGait::Jogging;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config|Character Movement", Meta = (DisplayName = "Properties"))
	FCLCharacterMovementProperties CharacterMovementProps;

	UPROPERTY(EditDefaultsOnly, Category = "Config|Character Movement", Meta = (DisplayName = "Gait Settings"))
	FCLGaitSettingsCollection GaitSettings;

	float GetMaxWalkingSpeed() const;
	float GetMaxCustomSpeed() const;
	FORCEINLINE bool IsCustomMovementMode(const ECLCustomMovementMode InCustomMovementMode) const { return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode; }
	FORCEINLINE void SetCustomMovementMode(const ECLCustomMovementMode InNewCustomMovementMode) { SetMovementMode(MOVE_Custom, InNewCustomMovementMode); }
	void SetStance(const ECLStance InStance);
	void SetGait(const ECLGait InGait);
	
	//~ UCharacterMovementComponent Begin
public:
	virtual void BeginPlay() override;
	virtual bool IsCrouching() const override;
	virtual void Crouch(bool bClientSimulation = false) override;
	virtual void UnCrouch(bool bClientSimulation = false) override;
	virtual float GetMaxSpeed() const override;
	virtual void PostLoad() override;
	virtual void SetUpdatedComponent(USceneComponent* NewUpdatedComponent) override;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
protected:
	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;
	virtual void ProcessLanded(const FHitResult& Hit, float remainingTime, int32 Iterations) override;
	//~ UCharacterMovementComponent End
};
