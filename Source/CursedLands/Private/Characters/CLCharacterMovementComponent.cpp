// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/CLCharacterMovementComponent.h"

void UCLCharacterMovementComponent::StartSprinting()
{
	if (!CanSprintInCurrentState())
	{
		return;
	}
	
	bWantsToSprint = true;
}

void UCLCharacterMovementComponent::StopSprinting()
{
	bWantsToSprint = false;
}

void UCLCharacterMovementComponent::PhysSprinting(float DeltaTime, int32 Iterations)
{
	PhysWalking(DeltaTime, Iterations);
}

//~ UCharacterMovementComponent Begin

bool UCLCharacterMovementComponent::CanAttemptJump() const
{
	return Super::CanAttemptJump() || IsSprinting();
}

float UCLCharacterMovementComponent::GetMaxBrakingDeceleration() const
{
	if (MovementMode != MOVE_Custom)
	{
		return Super::GetMaxBrakingDeceleration();
	}
	
	switch (CustomMovementMode)
	{
	case CMOVE_Sprinting:
		return BrakingDecelerationWalking;
	default:
		return 0.f;
	}
}

float UCLCharacterMovementComponent::GetMaxSpeed() const
{
	if (MovementMode != MOVE_Custom)
	{
		return Super::GetMaxSpeed();
	}
	
	switch (CustomMovementMode)
	{
	case CMOVE_Sprinting:
		return MaxSprintSpeed;
	default:
		return MaxCustomMovementSpeed;
	}
}

void UCLCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// We want to always evaluate FallHeight as we use it for multiple falling animations 
	if (IsFalling())
	{
		FallHeight = FallBeginZ - GetOwner()->GetActorLocation().Z;
	}
}

void UCLCharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	// The Super logic is relevant only for out of the box crouching
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
	
	const bool bIsSprinting { IsCustomMovementMode(CMOVE_Sprinting) };
	
	if (bWantsToSprint
		&& !Velocity.IsNearlyZero()
		&& IsMovingOnGround()
		)
	{
		SetCustomMovementMode(CMOVE_Sprinting);
	}

	if (!bWantsToSprint && bIsSprinting)
	{
		SetMovementMode(DefaultLandMovementMode);
	}

	FFindFloorResult FloorResult;
	FindFloor(UpdatedComponent->GetComponentLocation(), FloorResult, false);

	// If trying to sprint or already sprinting while falling, then set mode to falling and disable sprinting.
	// This is important to cover both sudden falling (checking IsSprinting) and jumping (checking WantsToSprint).
	if ((bWantsToSprint || bIsSprinting) && !FloorResult.bWalkableFloor)
	{
		SetMovementMode(MOVE_Falling);
		bWantsToSprint = CharacterMovementProps.bReturnSprintingAfterFall;
	}
}

// bool UCLCharacterMovementComponent::IsMovingOnGround() const
// {
// 	return Super::IsMovingOnGround() || IsSprinting();
// }

void UCLCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (PreviousMovementMode != MOVE_Falling && MovementMode == MOVE_Falling)
	{
		FallBeginZ = GetActorLocation().Z;
	}
}

void UCLCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	Super::PhysCustom(DeltaTime, Iterations);
	
	switch (CustomMovementMode)
	{
	case CMOVE_Sprinting:
		PhysSprinting(DeltaTime, Iterations);
		break;
	default:
		checkNoEntry();
		break;
	}
}

//~ UCharacterMovementComponent End
