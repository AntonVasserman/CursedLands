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
enum class ECLMovementWalkingMode : uint8
{
	None		UMETA(DisplayName = "None"),
	Idle		UMETA(DisplayName = "Idle"),
	Walking		UMETA(DisplayName = "Walking"),
	Jogging		UMETA(DisplayName = "Jogging"),
	Sprinting	UMETA(DisplayName = "Sprinting"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMovementWalkingModeChanged, ECLMovementWalkingMode, PreviousMovementWalkingMode, ECLMovementWalkingMode, MovementWalkingMode);

USTRUCT(BlueprintType)
struct FCLCharacterMovementProperties
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Properties|Walking")
	uint8 bCanEverSprint:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Properties|Walking")
	uint8 bStopSprintingOnNotMovingOnGround:1;

	FCLCharacterMovementProperties()
		: bCanEverSprint(false), bStopSprintingOnNotMovingOnGround(false)
	{
	}
};

UCLASS()
class CURSEDLANDS_API UCLCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	// TODO (202504-2): Design a solution where this isn't public
	uint8 bWantsToSprint:1 { false };
	FOnMovementWalkingModeChanged OnMovementWalkingModeChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Falling")
	FORCEINLINE float GetFallHeight() const { return FallHeight; }
	
	FORCEINLINE bool CanEverSprint() const { return CharacterMovementProps.bCanEverSprint; }
	FORCEINLINE bool CanSprintInCurrentState() const { return CanEverSprint() && !Velocity.IsNearlyZero() && IsMovingOnGround(); }
	FORCEINLINE ECLMovementWalkingMode GetMovementWalkingMode() const { return MovementWalkingMode; }
	bool IsSprinting() const;
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Walking|Sprint")
	void Sprint();
	UFUNCTION(BlueprintCallable, Category = "Character Movement|Walking|Sprint")
	void UnSprint();
	
private:
	UPROPERTY(Transient, DuplicateTransient)
	TObjectPtr<ACLPlayerCharacter> PlayerCharacterOwner;
	
	UPROPERTY(BlueprintReadOnly, Category = "Character Movement|Walking", Meta = (AllowPrivateAccess))
	ECLMovementWalkingMode MovementWalkingMode = ECLMovementWalkingMode::Idle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config|Character Movement", Meta = (DisplayName = "Properties"))
	FCLCharacterMovementProperties CharacterMovementProps;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config|Character Movement|Walking|Sprint")
	float MaxWalkSpeedSprinting { 900.f };

	float FallHeight = 0.f;
	float FallBeginZ = 0.f;

	float GetMaxWalkingSpeed() const;
	float GetMaxCustomSpeed() const;
	FORCEINLINE bool IsCustomMovementMode(const ECLCustomMovementMode InCustomMovementMode) const { return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode; }
	FORCEINLINE void SetCustomMovementMode(const ECLCustomMovementMode InNewCustomMovementMode) { SetMovementMode(MOVE_Custom, InNewCustomMovementMode); }
	void SetMovementWalkingMode(const ECLMovementWalkingMode InMovementWalkingMode);
	
	//~ UCharacterMovementComponent Begin
public:
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
