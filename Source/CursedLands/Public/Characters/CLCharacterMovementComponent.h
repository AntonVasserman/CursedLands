// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CLCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum ECLCustomMovementMode : uint8
{
	CMOVE_None		UMETA(DisplayName = "None"),
	CMOVE_Sprinting	UMETA(DisplayName = "Sprinting"),
};

USTRUCT(BlueprintType)
struct FCLCharacterMovementProperties
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Properties | Sprinting")
	uint8 bCanEverSprint:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Properties | Sprinting")
	uint8 bReturnSprintingAfterFall:1;

	FCLCharacterMovementProperties()
		: bCanEverSprint(false), bReturnSprintingAfterFall(false)
	{
	}
};

UCLASS()
class CURSEDLANDS_API UCLCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	FORCEINLINE bool CanEverSprint() const { return CharacterMovementProps.bCanEverSprint; }
	FORCEINLINE bool CanSprintInCurrentState() const { return CanEverSprint() && IsMovingOnGround(); }
	FORCEINLINE bool IsSprinting() const { return IsCustomMovementMode(CMOVE_Sprinting) && UpdatedComponent; }
	// Essentially the Start/Stop functions are just for a cleaner API, they are just delegations to SetMovementMode
	UFUNCTION(BlueprintCallable, Category = "Character Movement | Falling")
	FORCEINLINE float GetFallHeight() const { return FallHeight; }
	UFUNCTION(BlueprintCallable, Category = "Character Movement | Sprinting")
	void StartSprinting();
	UFUNCTION(BlueprintCallable, Category = "Character Movement | Sprinting")
	void StopSprinting();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Config | Character Movement", Meta = (DisplayName = "Properties"))
	FCLCharacterMovementProperties CharacterMovementProps;
	
	bool bWantsToSprint { false };
	UPROPERTY(EditDefaultsOnly, Category = "Config | Character Movement | Sprinting")
	float MaxSprintSpeed { 900.f };

	float FallHeight = 0.f;
	float FallBeginZ = 0.f;
	
	FORCEINLINE bool IsCustomMovementMode(const ECLCustomMovementMode InCustomMovementMode) const { return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode; }
	void PhysSprinting(float DeltaTime, int32 Iterations);
	FORCEINLINE void SetCustomMovementMode(const ECLCustomMovementMode InNewCustomMovementMode) { SetMovementMode(MOVE_Custom, InNewCustomMovementMode); }
	
	
	//~ UCharacterMovementComponent Begin
public:
	virtual bool CanAttemptJump() const override;
	virtual float GetMaxBrakingDeceleration() const override;
	virtual float GetMaxSpeed() const override;
	// virtual bool IsMovingOnGround() const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;

protected:
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;
	//~ UCharacterMovementComponent End
};
