// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/Components/CLCharacterMovementComponent.h"

#include "Characters/CLPlayerCharacter.h"

FCLGaitSettings UCLCharacterMovementComponent::GetGaitSettings(const ECLGait InGait) const
{
	return GaitSettings.GetSettingsForGait(InGait);
}

void UCLCharacterMovementComponent::RequestWalking()
{
	if (CanWalkInCurrentState())
	{
		SetGait(ECLGait::Walking);
	}
}

void UCLCharacterMovementComponent::RequestJogging()
{
	SetGait(ECLGait::Jogging);
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

void UCLCharacterMovementComponent::SetStance(const ECLStance InStance)
{
	const ECLStance PrevStance = Stance;
	Stance = InStance;

	OnStanceChanged.Broadcast(PrevStance, Stance);
}

void UCLCharacterMovementComponent::SetGait(const ECLGait InGait)
{
	const ECLGait PrevGait = Gait;
	Gait = InGait;

	// Update CMC Settings
	const FCLGaitSettings InGaitSettings = GaitSettings.GetSettingsForGait(Gait);
	MaxWalkSpeed = MaxWalkSpeedCrouched = InGaitSettings.MaxWalkingSpeed;
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

	SetStance(Stance);
	// When game begin Set Gait to the default value, this will populate all the relevant fields and run any relevant callbacks
	SetGait(Gait);
}

bool UCLCharacterMovementComponent::IsCrouching() const
{
	return Super::IsCrouching() && Stance == ECLStance::Crouching;
}

void UCLCharacterMovementComponent::Crouch(bool bClientSimulation)
{
	Super::Crouch(bClientSimulation);

	SetStance(ECLStance::Crouching);
	SetGait(ECLGait::Walking); // Walking is the default Crouching Gait
}

void UCLCharacterMovementComponent::UnCrouch(bool bClientSimulation)
{
	Super::UnCrouch(bClientSimulation);

	SetStance(ECLStance::Standing);
	SetGait(ECLGait::Jogging); // Jogging is the default Standing Gait
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

void UCLCharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	// The Super logic is relevant only for out of the box crouching
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);

	if (Gait == ECLGait::Sprinting)
	{
		if (!bWantsToSprint)
		{
			UnSprint();
		}
	}
	else if (bWantsToSprint && CanSprintInCurrentState())
	{
		Sprint();
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

void UCLCharacterMovementComponent::ProcessLanded(const FHitResult& Hit, float remainingTime, int32 Iterations)
{
	Super::ProcessLanded(Hit, remainingTime, Iterations);

	if (Gait == ECLGait::Sprinting && CharacterMovementProps.bStopSprintingOnLanding)
	{
		bWantsToSprint = false;
		UnSprint();
	}
}

//~ UCharacterMovementComponent End
