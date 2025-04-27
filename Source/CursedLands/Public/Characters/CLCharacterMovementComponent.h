// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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
enum class ECLGait : uint8
{
	Jogging		UMETA(DisplayName = "Jogging"),
	Walking		UMETA(DisplayName = "Walking"),
	Sprinting	UMETA(DisplayName = "Sprinting"),
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGaitChanged, ECLGait, PreviousGait, ECLGait, Gait);

USTRUCT(BlueprintType)
struct FCLCharacterMovementProperties
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Properties|Walking|Walk")
	uint8 bCanEverWalk:1 = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Properties|Walking|Sprint")
	uint8 bCanEverSprint:1 = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Properties|Walking|Sprint")
	uint8 bStopSprintingOnNotMovingOnGround:1 = false;
};

USTRUCT()
struct FCLGaitSettings
{
	GENERATED_USTRUCT_BODY()

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

USTRUCT(BlueprintType)
struct FCLGaitSettingsCollection
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	FCLGaitSettings Walking;

	UPROPERTY(EditDefaultsOnly)
	FCLGaitSettings Jogging;

	UPROPERTY(EditDefaultsOnly)
	FCLGaitSettings Sprinting;
	
	const FCLGaitSettings& GetSettingsForGait(const ECLGait InGait) const
	{
		switch (InGait)
		{
		case ECLGait::Walking:
			return Walking;
		case ECLGait::Jogging:
			return Jogging;
		case ECLGait::Sprinting:
			return Sprinting;
		default:
			checkNoEntry();
			return Jogging;
		}
	}
};

UCLASS()
class CURSEDLANDS_API UCLCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	FOnGaitChanged OnGaitChanged;

	FORCEINLINE ECLGait GetGait() const { return Gait; }
	FCLGaitSettings GetGaitSettings(const ECLGait InGait) const;
	
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Falling")
	FORCEINLINE float GetFallHeight() const { return FallHeight; }

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
	bool IsSprinting() const;
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Walking|Sprint")
	void RequestSprinting();
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Walking|Sprint")
	void RequestUnSprinting();
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Walking|Sprint")
	void Sprint();
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Walking|Sprint")
	void UnSprint();
	
private:
	uint8 bWantsToSprint:1 { false };
	
	UPROPERTY(Transient, DuplicateTransient)
	TObjectPtr<ACLPlayerCharacter> PlayerCharacterOwner;
	
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion", Meta = (AllowPrivateAccess))
	ECLGait Gait = ECLGait::Jogging;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config|Character Movement", Meta = (DisplayName = "Properties"))
	FCLCharacterMovementProperties CharacterMovementProps;

	UPROPERTY(EditDefaultsOnly, Category = "Config|Character Movement", Meta = (DisplayName = "Gait Settings"))
	FCLGaitSettingsCollection GaitSettings;

	float FallHeight = 0.f;
	float FallBeginZ = 0.f;

	float GetMaxWalkingSpeed() const;
	float GetMaxCustomSpeed() const;
	FORCEINLINE bool IsCustomMovementMode(const ECLCustomMovementMode InCustomMovementMode) const { return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode; }
	FORCEINLINE void SetCustomMovementMode(const ECLCustomMovementMode InNewCustomMovementMode) { SetMovementMode(MOVE_Custom, InNewCustomMovementMode); }
	void SetGait(const ECLGait InGait);
	
	//~ UCharacterMovementComponent Begin
public:
	virtual void BeginPlay() override;
	virtual float GetMaxSpeed() const override;
	virtual void PostLoad() override;
	virtual void SetUpdatedComponent(USceneComponent* NewUpdatedComponent) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
protected:
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;
	//~ UCharacterMovementComponent End
};
