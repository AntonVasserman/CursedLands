// Copyright Anton Vasserman, All Rights Reserved.


#include "Animation/CLPlayerCharacterAnimInstance.h"

#include "CLLogChannels.h"
#include "KismetAnimationLibrary.h"
#include "Characters/CLPlayerCharacter.h"

static TAutoConsoleVariable CVarShowDebugCLPlayerAnimInstance(
	TEXT("CLShowDebug.PlayerCharacterAnimInstance"),
	false,
	TEXT("Shows the Debug information of the CLPlayerAnimInstance class"),
	ECVF_Default);

ECLCardinalDirection UCLPlayerCharacterAnimInstance::EvaluateCardinalDirection(const float InCardinalDirectionAngle, const float InBackwardMin, const float InBackwardMax,
	const float InForwardMin, const float InForwardMax, const ECLCardinalDirection InCardinalDirection, const float InDeadzone)
{
	// Handling Deadzone first
	bool bIsInDeadzone = false;
	switch (InCardinalDirection)
	{
	case ECLCardinalDirection::Forward:
		bIsInDeadzone = InCardinalDirectionAngle >= InForwardMin - InDeadzone && InCardinalDirectionAngle <= InForwardMax + InDeadzone; 
		break;
	case ECLCardinalDirection::Backward:
		bIsInDeadzone = InCardinalDirectionAngle < InBackwardMin + InDeadzone || InCardinalDirectionAngle > InBackwardMax - InDeadzone; 
		break;
	case ECLCardinalDirection::Right:
		bIsInDeadzone = InCardinalDirectionAngle > InForwardMax - InDeadzone && InCardinalDirectionAngle < InBackwardMax + InDeadzone;
		break;
	case ECLCardinalDirection::Left:
		bIsInDeadzone = InCardinalDirectionAngle > InBackwardMin - InDeadzone && InCardinalDirectionAngle < InForwardMin + InDeadzone;
		break;
	default:
		checkNoEntry();
	}

	if (bIsInDeadzone)
	{
		return InCardinalDirection;
	}
	
	// Evaluate Direction outside of deadzone
	if (InCardinalDirectionAngle >= InForwardMin && InCardinalDirectionAngle <= InForwardMax) // Forward Direction
	{
		return ECLCardinalDirection::Forward;
	}
	if (InCardinalDirectionAngle < InBackwardMin || InCardinalDirectionAngle > InBackwardMax) // Backward Direction
	{
		return ECLCardinalDirection::Backward;
	}
	if (InCardinalDirectionAngle > 0) // Right Direction
	{
		return ECLCardinalDirection::Right;
	}
	return ECLCardinalDirection::Left;
}

void UCLPlayerCharacterAnimInstance::UpdateLocomotionData(const ACLPlayerCharacter* InPlayerCharacter)
{
	// Evaluate CardinalDirectionAngle
	const FRotator PlayerCharacterRotation = InPlayerCharacter->GetActorRotation();
	const FVector PlayerCharacterVelocity2D = InPlayerCharacter->GetVelocity() * FVector(1, 1, 0);
	CardinalDirectionAngle = UKismetAnimationLibrary::CalculateDirection(PlayerCharacterVelocity2D, PlayerCharacterRotation);

	// Evaluate CardinalDirection
	CardinalDirection = EvaluateCardinalDirection(CardinalDirectionAngle, -130.f, 130.f, -50.f, 50.f, CardinalDirection, 20.f);

	// Evaluate Gait
	Gait = MovementModeGaitMap.FindChecked(InPlayerCharacter->GetCLCharacterMovement()->GetMovementWalkingMode());
}

//~ UCLAnimInstance Begin

void UCLPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (ACLPlayerCharacter* OwningPlayerCharacter = Cast<ACLPlayerCharacter>(GetOwningActor()))
	{
		PlayerCharacter = OwningPlayerCharacter;
		FallHeightForMinFallDamage = PlayerCharacter->GetFallHeightForMinFallDamage();
		FallHeightForMaxFallDamage = PlayerCharacter->GetFallHeightForMaxFallDamage();
	}
}

void UCLPlayerCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}

void UCLPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (PlayerCharacter)
	{
		MovementMode = PlayerCharacter->GetMovementMode();
		FallHeight = PlayerCharacter->GetCLCharacterMovement()->GetFallHeight();

		UpdateLocomotionData(PlayerCharacter);
	}

	if (CVarShowDebugCLPlayerAnimInstance->GetBool() && GEngine)
	{
		const FColor TextColor = FColor::Red;
		const FVector2D TextScale = FVector2D(1.5f, 1.5f);

		// Putting them in reverse order since the first added is actually last on screen
		GEngine->AddOnScreenDebugMessage(10, 0.0f, TextColor, FString::Printf(TEXT("Gait: %s"), *StaticEnum<ECLGait>()->GetAuthoredNameStringByValue(static_cast<int64>(Gait))), false, TextScale);
		GEngine->AddOnScreenDebugMessage(9, 0.0f, TextColor, FString::Printf(TEXT("CardinalDirectionAngle: %f"), CardinalDirectionAngle), false, TextScale);
		GEngine->AddOnScreenDebugMessage(8, 0.0f, TextColor, FString::Printf(TEXT("CardinalDirection: %s"), *StaticEnum<ECLCardinalDirection>()->GetAuthoredNameStringByValue(static_cast<int64>(CardinalDirection))), false, TextScale);
		GEngine->AddOnScreenDebugMessage(7, 0.0f, TextColor, FString::Printf(TEXT("FallHeight: %f"), FallHeight), false, TextScale);
		GEngine->AddOnScreenDebugMessage(6, 0.0f, TextColor, FString::Printf(TEXT("MovementMode: %s"), *StaticEnum<ECLPlayerCharacterMovementMode>()->GetAuthoredNameStringByValue(static_cast<int64>(MovementMode))), false, TextScale);
		
		GEngine->AddOnScreenDebugMessage(5, 0.0f, TextColor, FString::Printf(TEXT("bFalling: %d"), bFalling), false, TextScale);
		GEngine->AddOnScreenDebugMessage(4, 0.0f, TextColor, FString::Printf(TEXT("bAlive: %d"), bAlive), false, TextScale);
		GEngine->AddOnScreenDebugMessage(3, 0.0f, TextColor, FString::Printf(TEXT("bShouldMove: %d"), bShouldMove), false, TextScale);
		GEngine->AddOnScreenDebugMessage(2, 0.0f, TextColor, FString::Printf(TEXT("Direction: %f"), Direction), false, TextScale);
		GEngine->AddOnScreenDebugMessage(1, 0.0f, TextColor, FString::Printf(TEXT("GroundSpeed: %f"), GroundSpeed), false, TextScale);
		GEngine->AddOnScreenDebugMessage(0, 0.0f, TextColor, FString::Printf(TEXT("Velocity: %s"), *Velocity.ToString()), false, TextScale);
	}
}

//~ UCLAnimInstance End
