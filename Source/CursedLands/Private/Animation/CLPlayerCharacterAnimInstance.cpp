// Copyright Anton Vasserman, All Rights Reserved.


#include "Animation/CLPlayerCharacterAnimInstance.h"

#include "AnimCharacterMovementLibrary.h"
#include "Characters/CLPlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

static TAutoConsoleVariable CVarShowDebugCLPlayerAnimInstance(
	TEXT("CLShowDebug.PlayerCharacterAnimInstance"),
	false,
	TEXT("Shows the Debug information of the CLPlayerAnimInstance class"),
	ECVF_Default);

void UCLPlayerCharacterAnimInstance::UpdateFallData()
{
	Super::UpdateFallData();

	FallHeight = PlayerCharacter->GetCLCharacterMovement()->GetFallHeight();
}

void UCLPlayerCharacterAnimInstance::UpdateAccelerationData(const ACLPlayerCharacter* InPlayerCharacter)
{
	Acceleration = InPlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration();
	Acceleration2D = FVector(Acceleration.X, Acceleration.Y, 0.f);
	bAccelerating = UKismetMathLibrary::NearlyEqual_FloatFloat(UKismetMathLibrary::VSizeXY(Acceleration2D), 0.f) == false;
}

void UCLPlayerCharacterAnimInstance::UpdateLocomotionData(const ACLPlayerCharacter* InPlayerCharacter)
{
	MovementMode = PlayerCharacter->GetMovementMode();
	CardinalDirectionAngle = InPlayerCharacter->GetCardinalDirectionAngle();
	CardinalDirection = InPlayerCharacter->GetCardinalDirection();
	Gait = InPlayerCharacter->GetCLCharacterMovement()->GetGait();
}

void UCLPlayerCharacterAnimInstance::UpdateRotationData(const float DeltaSeconds, const ACLPlayerCharacter* InPlayerCharacter)
{
	// Evaluate new PlayerCharacterRotation
	LastYawDelta = InPlayerCharacter->GetActorRotation().Yaw - PlayerCharacterRotation.Yaw;
	PlayerCharacterRotation = InPlayerCharacter->GetActorRotation();
	
	// Evaluate LeanAngle
	if (Gait == ECLGait::Walking)
	{
		LeanAngle = 0.f;
	}
	else
	{
		float NormalizedLeanAngle = UKismetMathLibrary::SafeDivide(LastYawDelta, DeltaSeconds);
		NormalizedLeanAngle = NormalizedLeanAngle * 0.375f; // Multiply by a smaller value to clamp the lean angle
		NormalizedLeanAngle = FMath::Clamp(NormalizedLeanAngle, -90.0f, 90.0f);
		NormalizedLeanAngle = CardinalDirection != ECLCardinalDirection::Backward ? NormalizedLeanAngle : -NormalizedLeanAngle;

		switch (Gait)
		{
		case ECLGait::Jogging:
			NormalizedLeanAngle *= 0.8; // For jogging cap at a bit of a lower value than maximum
			break;
		case ECLGait::Sprinting:
			NormalizedLeanAngle *= 1.f; // Basically have no effect
			break;
		default:
			checkNoEntry();
			break;
		}
	
		LeanAngle = CardinalDirection != ECLCardinalDirection::Backward ? NormalizedLeanAngle : -NormalizedLeanAngle;
	}

	// Ignore evaluated values if this is the first tick.
	if (bFirstUpdate)
	{
		LastYawDelta = 0.f;
		LeanAngle = 0.f;
	}
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

void UCLPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!PlayerCharacter)
	{
		return;
	}
	
	UpdateAccelerationData(PlayerCharacter);
	UpdateLocomotionData(PlayerCharacter);
	UpdateRotationData(DeltaSeconds, PlayerCharacter);

	if (CVarShowDebugCLPlayerAnimInstance->GetBool() && GEngine)
	{
		const FColor TextColor = FColor::Red;
		const FVector2D TextScale = FVector2D(1.5f, 1.5f);

		// Putting them in reverse order since the first added is actually last on screen
		GEngine->AddOnScreenDebugMessage(54, 0.0f, TextColor, FString::Printf(TEXT("LeanAngle: %f"), LeanAngle), false, TextScale);
		
		GEngine->AddOnScreenDebugMessage(43, 0.0f, TextColor, FString::Printf(TEXT("Gait: %s"), *StaticEnum<ECLGait>()->GetAuthoredNameStringByValue(static_cast<int64>(Gait))), false, TextScale);
		GEngine->AddOnScreenDebugMessage(42, 0.0f, TextColor, FString::Printf(TEXT("CardinalDirectionAngle: %f"), CardinalDirectionAngle), false, TextScale);
		GEngine->AddOnScreenDebugMessage(41, 0.0f, TextColor, FString::Printf(TEXT("CardinalDirection: %s"), *StaticEnum<ECLCardinalDirection>()->GetAuthoredNameStringByValue(static_cast<int64>(CardinalDirection))), false, TextScale);
		GEngine->AddOnScreenDebugMessage(40, 0.0f, TextColor, FString::Printf(TEXT("MovementMode: %s"), *StaticEnum<ECLPlayerCharacterMovementMode>()->GetAuthoredNameStringByValue(static_cast<int64>(MovementMode))), false, TextScale);

		GEngine->AddOnScreenDebugMessage(32, 0.0f, TextColor, FString::Printf(TEXT("Accelerating: %d"), bAccelerating), false, TextScale);
		GEngine->AddOnScreenDebugMessage(31, 0.0f, TextColor, FString::Printf(TEXT("Acceleration2D: %s"), *Acceleration2D.ToString()), false, TextScale);
		GEngine->AddOnScreenDebugMessage(30, 0.0f, TextColor, FString::Printf(TEXT("Acceleration: %s"), *Acceleration.ToString()), false, TextScale);
		
		GEngine->AddOnScreenDebugMessage(21, 0.0f, TextColor, FString::Printf(TEXT("FallHeight: %f"), FallHeight), false, TextScale);
		GEngine->AddOnScreenDebugMessage(20, 0.0f, TextColor, FString::Printf(TEXT("Falling: %d"), bFalling), false, TextScale);

		GEngine->AddOnScreenDebugMessage(12, 0.0f, TextColor, FString::Printf(TEXT("Velocity2DSize: %f"), Velocity2DSize), false, TextScale);
		GEngine->AddOnScreenDebugMessage(11, 0.0f, TextColor, FString::Printf(TEXT("Velocity2D: %s"), *Velocity2D.ToString()), false, TextScale);
		GEngine->AddOnScreenDebugMessage(10, 0.0f, TextColor, FString::Printf(TEXT("Velocity: %s"), *Velocity.ToString()), false, TextScale);

		// Draw Direction/Velocity
		const FVector PlayerCharacterLocation = PlayerCharacter->GetActorLocation();
		const FVector DirectionalArrowLineStart = FVector(PlayerCharacterLocation.X, PlayerCharacterLocation.Y, PlayerCharacterLocation.Z - 60.f);
		const FVector DirectionalArrowLineEnd = DirectionalArrowLineStart + Velocity;
		DrawDebugDirectionalArrow(GetWorld(), DirectionalArrowLineStart, DirectionalArrowLineEnd, 5.f, FColor::Red, false, -1, 0, 2.5f);

		// Draw Predicted Ground Movement Stop Location
		// UAnimCharacterMovementLibrary::PredictGroundMovementStopLocation
		FVector StopLocationPrediction = UAnimCharacterMovementLibrary::PredictGroundMovementStopLocation(
			Velocity2D,
			PlayerCharacter->GetCharacterMovement()->bUseSeparateBrakingFriction,
			PlayerCharacter->GetCharacterMovement()->BrakingFriction,
			PlayerCharacter->GetCharacterMovement()->GroundFriction,
			PlayerCharacter->GetCharacterMovement()->BrakingFrictionFactor,
			PlayerCharacter->GetCharacterMovement()->BrakingDecelerationWalking);
		DrawDebugSphere(GetWorld(), PlayerCharacter->GetActorLocation() + StopLocationPrediction, 10.f, 32, FColor::Green, false, -1, 0, 2.5f);
	}

	bFirstUpdate = false;
}

//~ UCLAnimInstance End
