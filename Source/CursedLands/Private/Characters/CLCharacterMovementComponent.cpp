// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/CLCharacterMovementComponent.h"

#include "Characters/CLPlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

FCLGaitSettings UCLCharacterMovementComponent::GetGaitSettings(const ECLGait InGait) const
{
	return GaitSettings.GetSettingsForGait(InGait);
}

bool UCLCharacterMovementComponent::IsSprinting() const
{
	check(PlayerCharacterOwner);
	return Gait == ECLGait::Sprinting;
}

void UCLCharacterMovementComponent::RequestSprinting()
{
	bWantsToSprint = true;
}

void UCLCharacterMovementComponent::RequestUnSprinting()
{
	bWantsToSprint = false;
}

void UCLCharacterMovementComponent::Sprint()
{
	SetGait(ECLGait::Sprinting);
}

void UCLCharacterMovementComponent::UnSprint()
{
	SetGait(ECLGait::Jogging);
}

float UCLCharacterMovementComponent::GetMaxWalkingSpeed() const
{
	if (IsCrouching()) { return MaxWalkSpeedCrouched; }
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

	// Update CMC Settings
	const FCLGaitSettings InGaitSettings = GaitSettings.GetSettingsForGait(Gait);
	MaxWalkSpeed = InGaitSettings.MaxWalkingSpeed;
	MaxAcceleration = InGaitSettings.MaxAcceleration;
	BrakingDecelerationWalking = InGaitSettings.BrakingDeceleration;
	BrakingFrictionFactor = InGaitSettings.BrakingFrictionFactor;
	BrakingFriction = InGaitSettings.BrakingFriction;
	bUseSeparateBrakingFriction = InGaitSettings.bUseSeparateBrakingFriction;
	
	OnGaitChanged.Broadcast(PrevGait, Gait);
}

//~ UCharacterMovementComponent Begin

void UCLCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// When game begin Set Gait to the default value, this will populate all the relevant fields and run any relevant callbacks
	SetGait(Gait);
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
			UnSprint();
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
