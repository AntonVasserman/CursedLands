// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/CLCharacterMovementComponent.h"

#include "Characters/CLPlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

bool UCLCharacterMovementComponent::IsSprinting() const
{
	check(PlayerCharacterOwner);
	return PlayerCharacterOwner->bIsSprinting;
}

void UCLCharacterMovementComponent::Sprint()
{
	SetGait(ECLGait::Sprinting);
	check(PlayerCharacterOwner);
	PlayerCharacterOwner->bIsSprinting = true;
}

void UCLCharacterMovementComponent::UnSprint()
{
	SetGait(ECLGait::Jogging);
	check(PlayerCharacterOwner);
	PlayerCharacterOwner->bIsSprinting = false;
}

//~ UCharacterMovementComponent Begin

float UCLCharacterMovementComponent::GetMaxWalkingSpeed() const
{
	if (IsCrouching()) { return MaxWalkSpeedCrouched; }
	if (IsSprinting()) { return MaxWalkSpeedSprinting; }
	return MaxWalkSpeed;
}

float UCLCharacterMovementComponent::GetMaxCustomSpeed() const
{
	switch (CustomMovementMode)
	{
	// Placeholder for Max Speed of Custom Movement Modes
	case CMOVE_None: checkNoEntry();
	default: return MaxCustomMovementSpeed;
	}
}

void UCLCharacterMovementComponent::SetGait(const ECLGait InGait)
{
	const ECLGait PrevGait = Gait;
	Gait = InGait;
	OnGaitChanged.Broadcast(PrevGait, Gait);
}

float UCLCharacterMovementComponent::GetMaxSpeed() const
{
	if (MovementMode == MOVE_Walking) { return GetMaxWalkingSpeed(); }
	if (MovementMode == MOVE_Custom) { return GetMaxCustomSpeed(); }
	return Super::GetMaxSpeed();
}

void UCLCharacterMovementComponent::PostLoad()
{
	Super::PostLoad();
	PlayerCharacterOwner = Cast<ACLPlayerCharacter>(GetCharacterOwner());
}

void UCLCharacterMovementComponent::SetUpdatedComponent(USceneComponent* NewUpdatedComponent)
{
	Super::SetUpdatedComponent(NewUpdatedComponent);
	PlayerCharacterOwner = Cast<ACLPlayerCharacter>(GetCharacterOwner());
}

void UCLCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// We want to always evaluate FallHeight as we use it for multiple falling animations 
	if (IsFalling())
	{
		FallHeight = FallBeginZ - GetOwner()->GetActorLocation().Z;
	}

	const float CurrentGroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);
	if (CurrentGroundSpeed < MinAnalogWalkSpeed)
	{
		SetGait(ECLGait::Idle);
	}
	else if (CurrentGroundSpeed >= MinAnalogWalkSpeed && CurrentGroundSpeed < MaxWalkSpeed)
	{
		SetGait(ECLGait::Walking);
	}
	else if (CurrentGroundSpeed >= MaxWalkSpeed && CurrentGroundSpeed < MaxWalkSpeedSprinting)
	{
		SetGait(ECLGait::Jogging);
	}
	else
	{
		SetGait(ECLGait::Sprinting);
	}
}

void UCLCharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	// The Super logic is relevant only for out of the box crouching
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);

	if (IsSprinting())
	{
		if (!bWantsToSprint)
		{
			UnSprint();
		}
		else if (!IsMovingOnGround() && CharacterMovementProps.bStopSprintingOnNotMovingOnGround)
		{
			bWantsToSprint = false;
			PlayerCharacterOwner->bIsSprinting = false;
		}
	}
	else if (bWantsToSprint && CanSprintInCurrentState())
	{
		Sprint();
	}
}

void UCLCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (MovementMode == MOVE_Falling)
	{
		FallBeginZ = GetActorLocation().Z;
	}
}

void UCLCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	Super::PhysCustom(DeltaTime, Iterations);
	
	switch (CustomMovementMode)
	{
	case CMOVE_None:
	default:
		checkNoEntry();
		break;
	}
}

//~ UCharacterMovementComponent End
