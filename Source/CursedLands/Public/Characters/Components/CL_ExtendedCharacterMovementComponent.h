// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Compute/AgentMessage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CL_ExtendedCharacterMovementComponent.generated.h"

class ACL_PlayerCharacter;

UENUM(BlueprintType)
enum ECL_CustomMovementMode : uint8
{
	CMOVE_None		UMETA(DisplayName = "None"),
	// Placeholder for Custom Movement Modes
};

UENUM(BlueprintType)
enum class ECL_Stance : uint8
{
	Standing	UMETA(DisplayName = "Standing"),
	Crouching	UMETA(DisplayName = "Crouching"),
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStanceChanged, ECL_Stance, PreviousStance, ECL_Stance, Stance);

UENUM(BlueprintType)
enum class ECL_Gait : uint8
{
	Walking		UMETA(DisplayName = "Walking"),
	Jogging		UMETA(DisplayName = "Jogging"),
	Sprinting	UMETA(DisplayName = "Sprinting"),
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGaitChanged, ECL_Gait, PreviousGait, ECL_Gait, Gait);

USTRUCT(BlueprintType)
struct FCL_ExtendedCharacterMovementProperties
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
struct FCL_GaitSettings
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
class CURSEDLANDS_API UCL_GaitSettingsData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	FCL_GaitSettings GaitSettings;
};

UENUM(BlueprintType)
enum class ECL_GaitSettingsNotFoundHandlingOption : uint8
{
	UseDefault	UMETA(DisplayName = "Use Default", Description = "In case Gait Settings not found fallback to Default Gait Settings. Make sure Default Gait Settings are set."),
	Halt		UMETA(DisplayName = "Halt", Description = "In case Gait Settings not found execute check assertion to halt the engine."),
};

USTRUCT(BlueprintType)
struct FCL_GaitSettingsCollection
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, DisplayName = "Settings Mapping")
	TMap<ECL_Gait, TObjectPtr<const UCL_GaitSettingsData>> SettingsMap;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Settings Not Found Handling Option")
	ECL_GaitSettingsNotFoundHandlingOption SettingsNotFoundHandlingOption = ECL_GaitSettingsNotFoundHandlingOption::UseDefault;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Default Settings in case of Not Found", meta = (EditCondition = "SettingsNotFoundHandlingOption == ECL_GaitSettingsNotFoundHandlingOption::UseDefault", EditConditionHides))
	TObjectPtr<const UCL_GaitSettingsData> DefaultSettings;
	
	const FCL_GaitSettings& GetSettingsForGait(const ECL_Gait InGait) const
	{
		if (SettingsMap.Contains(InGait))
		{
			return SettingsMap[InGait]->GaitSettings;
		}

		switch (SettingsNotFoundHandlingOption)
		{
		case ECL_GaitSettingsNotFoundHandlingOption::UseDefault:
			return DefaultSettings->GaitSettings;
		case ECL_GaitSettingsNotFoundHandlingOption::Halt:
		default:
			checkNoEntry();
			break;
		}

		// We should never reach this point.
		return DefaultSettings->GaitSettings;
	}
};

UCLASS()
class CURSEDLANDS_API UCL_ExtendedCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	FOnStanceChanged OnStanceChanged;
	FOnGaitChanged OnGaitChanged;

	FORCEINLINE ECL_Stance GetStance() const { return Stance; }
	FORCEINLINE ECL_Gait GetGait() const { return Gait; }
	FCL_GaitSettings GetGaitSettings(const ECL_Gait InGait) const;

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
	TObjectPtr<ACL_PlayerCharacter> PlayerCharacterOwner;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion", Meta = (AllowPrivateAccess))
	ECL_Stance Stance = ECL_Stance::Standing;
	
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion", Meta = (AllowPrivateAccess))
	ECL_Gait Gait = ECL_Gait::Jogging;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config|Character Movement", Meta = (DisplayName = "Properties"))
	FCL_ExtendedCharacterMovementProperties CharacterMovementProps;

	UPROPERTY(EditDefaultsOnly, Category = "Config|Character Movement", Meta = (DisplayName = "Gait Settings"))
	FCL_GaitSettingsCollection GaitSettings;

	float GetMaxWalkingSpeed() const;
	float GetMaxCustomSpeed() const;
	FORCEINLINE bool IsCustomMovementMode(const ECL_CustomMovementMode InCustomMovementMode) const { return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode; }
	FORCEINLINE void SetCustomMovementMode(const ECL_CustomMovementMode InNewCustomMovementMode) { SetMovementMode(MOVE_Custom, InNewCustomMovementMode); }
	void SetStance(const ECL_Stance InStance);
	void SetGait(const ECL_Gait InGait);
	
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
