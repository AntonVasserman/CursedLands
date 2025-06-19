// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/Components/CL_ExtendedCharacterMovementComponent.h"

#include "Characters/CL_PlayerCharacter.h"

FCL_GaitSettings UCL_ExtendedCharacterMovementComponent::GetGaitSettings(const ECL_Gait InGait) const
{
	return GaitSettings.GetSettingsForGait(InGait);
}

void UCL_ExtendedCharacterMovementComponent::RequestWalking()
{
	if (CanWalkInCurrentState())
	{
		SetGait(ECL_Gait::Walking);
	}
}

void UCL_ExtendedCharacterMovementComponent::RequestJogging()
{
	SetGait(ECL_Gait::Jogging);
}

void UCL_ExtendedCharacterMovementComponent::RequestSprinting()
{
	bWantsToSprint = true;
}

void UCL_ExtendedCharacterMovementComponent::RequestUnSprinting()
{
	bWantsToSprint = false;
}

void UCL_ExtendedCharacterMovementComponent::Sprint()
{
	SetGait(ECL_Gait::Sprinting);
}

void UCL_ExtendedCharacterMovementComponent::UnSprint()
{
	SetGait(ECL_Gait::Jogging);
}

float UCL_ExtendedCharacterMovementComponent::GetMaxWalkingSpeed() const
{
	if (IsCrouching()) { return MaxWalkSpeedCrouched; }
	return MaxWalkSpeed;
}

float UCL_ExtendedCharacterMovementComponent::GetMaxCustomSpeed() const
{
	switch (CustomMovementMode)
	{
	// Placeholder for Max Speed of Custom Movement Modes
	case CMOVE_None: checkNoEntry();
	default: return MaxCustomMovementSpeed;
	}
}

void UCL_ExtendedCharacterMovementComponent::SetStance(const ECL_Stance InStance)
{
	const ECL_Stance PrevStance = Stance;
	Stance = InStance;

	OnStanceChanged.Broadcast(PrevStance, Stance);
}

void UCL_ExtendedCharacterMovementComponent::SetGait(const ECL_Gait InGait)
{
	const ECL_Gait PrevGait = Gait;
	Gait = InGait;

	// Update CMC Settings
	const FCL_GaitSettings InGaitSettings = GaitSettings.GetSettingsForGait(Gait);
	MaxWalkSpeed = MaxWalkSpeedCrouched = InGaitSettings.MaxWalkingSpeed;
	MaxAcceleration = InGaitSettings.MaxAcceleration;
	BrakingDecelerationWalking = InGaitSettings.BrakingDeceleration;
	BrakingFrictionFactor = InGaitSettings.BrakingFrictionFactor;
	BrakingFriction = InGaitSettings.BrakingFriction;
	bUseSeparateBrakingFriction = InGaitSettings.bUseSeparateBrakingFriction;
	
	OnGaitChanged.Broadcast(PrevGait, Gait);
}

//~ UCharacterMovementComponent Begin

void UCL_ExtendedCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	SetStance(Stance);
	// When game begin Set Gait to the default value, this will populate all the relevant fields and run any relevant callbacks
	SetGait(Gait);
}

bool UCL_ExtendedCharacterMovementComponent::IsCrouching() const
{
	return Super::IsCrouching() && Stance == ECL_Stance::Crouching;
}

void UCL_ExtendedCharacterMovementComponent::Crouch(bool bClientSimulation)
{
	Super::Crouch(bClientSimulation);

	SetStance(ECL_Stance::Crouching);
	SetGait(ECL_Gait::Walking); // Walking is the default Crouching Gait
}

void UCL_ExtendedCharacterMovementComponent::UnCrouch(bool bClientSimulation)
{
	Super::UnCrouch(bClientSimulation);

	SetStance(ECL_Stance::Standing);
	SetGait(ECL_Gait::Jogging); // Jogging is the default Standing Gait
}

float UCL_ExtendedCharacterMovementComponent::GetMaxSpeed() const
{
	if (MovementMode == MOVE_Walking) { return GetMaxWalkingSpeed(); }
	if (MovementMode == MOVE_Custom) { return GetMaxCustomSpeed(); }
	return Super::GetMaxSpeed();
}

void UCL_ExtendedCharacterMovementComponent::PostLoad()
{
	Super::PostLoad();
	PlayerCharacterOwner = Cast<ACL_PlayerCharacter>(GetCharacterOwner());
}

void UCL_ExtendedCharacterMovementComponent::SetUpdatedComponent(USceneComponent* NewUpdatedComponent)
{
	Super::SetUpdatedComponent(NewUpdatedComponent);
	PlayerCharacterOwner = Cast<ACL_PlayerCharacter>(GetCharacterOwner());
}

void UCL_ExtendedCharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	// The Super logic is relevant only for out of the box crouching
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);

	if (Gait == ECL_Gait::Sprinting)
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

void UCL_ExtendedCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
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

void UCL_ExtendedCharacterMovementComponent::ProcessLanded(const FHitResult& Hit, float remainingTime, int32 Iterations)
{
	Super::ProcessLanded(Hit, remainingTime, Iterations);

	if (Gait == ECL_Gait::Sprinting && CharacterMovementProps.bStopSprintingOnLanding)
	{
		bWantsToSprint = false;
		UnSprint();
	}
}

//~ UCharacterMovementComponent End
