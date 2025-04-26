// Copyright Anton Vasserman, All Rights Reserved.


#include "Animation/CLPlayerCharacterAnimInstance.h"

#include "AnimCharacterMovementLibrary.h"
#include "AVCollisionProfileStatics.h"
#include "AVDrawDebugStatics.h"
#include "KismetAnimationLibrary.h"
#include "Characters/CLPlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

static TAutoConsoleVariable CVarShowDebugCLPlayerAnimInstance(
	TEXT("CLShowDebug.PlayerCharacterAnimInstance"),
	false,
	TEXT("Shows the Debug information of the CLPlayerAnimInstance class"),
	ECVF_Default);

bool UCLPlayerCharacterAnimInstance::IsMovingPerpendicularToPivot() const
{
	switch (PivotCardinalDirection)
	{
		case ECLCardinalDirection::Forward:
		case ECLCardinalDirection::Backward:
			return CardinalDirection != ECLCardinalDirection::Forward && CardinalDirection != ECLCardinalDirection::Backward;
		case ECLCardinalDirection::Right:
		case ECLCardinalDirection::Left:
			return CardinalDirection != ECLCardinalDirection::Right && CardinalDirection != ECLCardinalDirection::Left;
		default:
			checkNoEntry();
			return false;
	}
}

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
	AccelerationAngle = UKismetAnimationLibrary::CalculateDirection(Acceleration2D, PlayerCharacterRotation);
}

void UCLPlayerCharacterAnimInstance::UpdateLocomotionData(const ACLPlayerCharacter* InPlayerCharacter)
{
	MovementMode = PlayerCharacter->GetMovementMode();
	CardinalDirectionAngle = InPlayerCharacter->GetCardinalDirectionAngle();
	CardinalDirectionAngleWithOffset = UKismetMathLibrary::NormalizeAxis(CardinalDirectionAngle - RootYawOffset); 
	LastCardinalDirection = CardinalDirection;
	CardinalDirection = InPlayerCharacter->GetCardinalDirection();
	const ECLGait NewGait = InPlayerCharacter->GetCLCharacterMovement()->GetGait();
	bGaitChanged = Gait != NewGait;
	Gait = NewGait;
	
}

void UCLPlayerCharacterAnimInstance::UpdateRootYawOffset(const float DeltaSeconds, const ACLPlayerCharacter* InPlayerCharacter)
{
	if (RootYawOffsetMode == ECLRootYawOffsetMode::Accumulate)
	{
		SetRootYawOffset(RootYawOffset - LastYawDelta);
	}

	if (RootYawOffsetMode == ECLRootYawOffsetMode::BlendOut)
	{
		const float NewRootYawOffset = FMath::FInterpTo(RootYawOffset, 0.f, DeltaSeconds, 10.f);
		SetRootYawOffset(NewRootYawOffset);
	}
	
	RootYawOffsetMode = ECLRootYawOffsetMode::BlendOut;
}

void UCLPlayerCharacterAnimInstance::SetRootYawOffset(const float InRootYawOffset)
{
	RootYawOffset = UKismetMathLibrary::NormalizeAxis(InRootYawOffset);
}

void UCLPlayerCharacterAnimInstance::ProcessTurnYawCurve()
{
	const float LastTurnYawCurveValue = TurnYawCurveValue;
	const float IsTurningCurveValue = GetCurveValue(IsTurningCurveName);
	if (UKismetMathLibrary::NearlyEqual_FloatFloat(IsTurningCurveValue, 0.f))
	{
		TurnYawCurveValue = 0.f;
	}
	else
	{
		const float RootRotationYawCurveValue = GetCurveValue(RootRotationYawCurveName);
		TurnYawCurveValue = UKismetMathLibrary::SafeDivide(RootRotationYawCurveValue, IsTurningCurveValue);
		UE_LOG(LogTemp, Warning, TEXT("RootRotationYawCurveValue: %f"), RootRotationYawCurveValue);
		if (LastTurnYawCurveValue != 0.f)
		{
			SetRootYawOffset(RootYawOffset - (TurnYawCurveValue - LastTurnYawCurveValue));
		}
	}
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
			NormalizedLeanAngle *= 1.f; // No effect
			break;
		default:
			checkNoEntry();
			break;
		}
	
		LeanAngle = CardinalDirection != ECLCardinalDirection::Backward ? NormalizedLeanAngle : -NormalizedLeanAngle;
	}

	// Ignore evaluated values if this is the first tick.
	if (bFirstThreadSafeUpdate)
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

void UCLPlayerCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (!PlayerCharacter)
	{
		return;
	}
	
	UpdateAccelerationData(PlayerCharacter);
	UpdateLocomotionData(PlayerCharacter);
	UpdateRotationData(DeltaSeconds, PlayerCharacter);
	UpdateRootYawOffset(DeltaSeconds, PlayerCharacter);

	bFirstThreadSafeUpdate = false;
}

void UCLPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// Await threadsafe update that populates most of the data
	if (!bFirstThreadSafeUpdate)
	{
		if (
			!bFirstThreadSafeUpdate &&
			CVarShowDebugCLPlayerAnimInstance->GetBool() &&
			GetWorld() && GetWorld()->IsPlayInEditor() &&
			GEngine
			)
		{
			const FColor TextColor = FColor::Red;
			const FVector2D TextScale = FVector2D(1.5f, 1.5f);

			// Putting them in reverse order since the first added is actually last on screen
			GEngine->AddOnScreenDebugMessage(71, 0.0f, TextColor, FString::Printf(TEXT("Root Yaw Offset Data::RootYawOffsetMode: %s"), *StaticEnum<ECLRootYawOffsetMode>()->GetAuthoredNameStringByValue(static_cast<int64>(RootYawOffsetMode))), false, TextScale);
			GEngine->AddOnScreenDebugMessage(70, 0.f, TextColor, FString::Printf(TEXT("Root Yaw Offset Data::RootYawOffset: %f"), RootYawOffset), false, TextScale);
			
			GEngine->AddOnScreenDebugMessage(60, 0.0f, TextColor, FString::Printf(TEXT("Rotation Data::LeanAngle: %f"), LeanAngle), false, TextScale);

			GEngine->AddOnScreenDebugMessage(51, 0.0f, TextColor, FString::Printf(TEXT("Locomotion SM Data::PivotCardinalDirection: %s"), *StaticEnum<ECLCardinalDirection>()->GetAuthoredNameStringByValue(static_cast<int64>(PivotCardinalDirection))), false, TextScale);
			GEngine->AddOnScreenDebugMessage(50, 0.0f, TextColor, FString::Printf(TEXT("Locomotion SM Data::PivotAcceleration2D: %s"), *PivotAcceleration2D.ToString()), false, TextScale);
			
			GEngine->AddOnScreenDebugMessage(43, 0.0f, TextColor, FString::Printf(TEXT("Locomotion Data::Gait: %s"), *StaticEnum<ECLGait>()->GetAuthoredNameStringByValue(static_cast<int64>(Gait))), false, TextScale);
			GEngine->AddOnScreenDebugMessage(42, 0.0f, TextColor, FString::Printf(TEXT("Locomotion Data::CardinalDirectionAngle: %f"), CardinalDirectionAngle), false, TextScale);
			GEngine->AddOnScreenDebugMessage(41, 0.0f, TextColor, FString::Printf(TEXT("Locomotion Data::CardinalDirection: %s"), *StaticEnum<ECLCardinalDirection>()->GetAuthoredNameStringByValue(static_cast<int64>(CardinalDirection))), false, TextScale);
			GEngine->AddOnScreenDebugMessage(40, 0.0f, TextColor, FString::Printf(TEXT("Locomotion Data::MovementMode: %s"), *StaticEnum<ECLPlayerCharacterMovementMode>()->GetAuthoredNameStringByValue(static_cast<int64>(MovementMode))), false, TextScale);

			GEngine->AddOnScreenDebugMessage(32, 0.0f, TextColor, FString::Printf(TEXT("Acceleration Data::AccelerationAngle: %f"), AccelerationAngle), false, TextScale);
			GEngine->AddOnScreenDebugMessage(31, 0.0f, TextColor, FString::Printf(TEXT("Acceleration Data::Accelerating: %d"), bAccelerating), false, TextScale);
			GEngine->AddOnScreenDebugMessage(30, 0.0f, TextColor, FString::Printf(TEXT("Acceleration Data::Acceleration: %s"), *Acceleration.ToString()), false, TextScale);
			
			GEngine->AddOnScreenDebugMessage(21, 0.0f, TextColor, FString::Printf(TEXT("Fall Data::FallHeight: %f"), FallHeight), false, TextScale);
			GEngine->AddOnScreenDebugMessage(20, 0.0f, TextColor, FString::Printf(TEXT("Fall Data::Falling: %d"), bFalling), false, TextScale);

			GEngine->AddOnScreenDebugMessage(11, 0.0f, TextColor, FString::Printf(TEXT("Velocity Data::Velocity2DSize: %f"), Velocity2DSize), false, TextScale);
			GEngine->AddOnScreenDebugMessage(10, 0.0f, TextColor, FString::Printf(TEXT("Velocity Data::Velocity: %s"), *Velocity.ToString()), false, TextScale);

			GEngine->AddOnScreenDebugMessage(1, 0.0f, TextColor, FString::Printf(TEXT("Location Data::CharacterLocationDeltaSizeXY: %f"), CharacterLocationDeltaSizeXY), false, TextScale);
			GEngine->AddOnScreenDebugMessage(0, 0.0f, TextColor, FString::Printf(TEXT("Location Data::CharacterLocation: %s"), *CharacterLocation.ToString()), false, TextScale);

			// Draw Direction/Velocity Vector
			const FVector DebugVectorStart = FVector(CharacterLocation.X, CharacterLocation.Y, CharacterLocation.Z - 60.f);
			const FVector VelocityDebugVectorEnd = DebugVectorStart + Velocity;
			UAVDrawDebugStatics::DrawDebugNamedDirectionalArrow(GetWorld(), DebugVectorStart, VelocityDebugVectorEnd, FString(TEXT("Velocity")), FColor::Green, 200.f);

			// Draw Acceleration Vector
			const FVector AccelerationDebugVectorEnd = DebugVectorStart + Acceleration;
			UAVDrawDebugStatics::DrawDebugNamedDirectionalArrow(GetWorld(), DebugVectorStart, AccelerationDebugVectorEnd, FString(TEXT("Acceleration")), FColor::Yellow, 200.f);

			// Draw Predicted Ground Movement Stop Location
			const FVector StopLocationPrediction = UAnimCharacterMovementLibrary::PredictGroundMovementStopLocation(
				Velocity2D,
				PlayerCharacter->GetCharacterMovement()->bUseSeparateBrakingFriction,
				PlayerCharacter->GetCharacterMovement()->BrakingFriction,
				PlayerCharacter->GetCharacterMovement()->GroundFriction,
				PlayerCharacter->GetCharacterMovement()->BrakingFrictionFactor,
				PlayerCharacter->GetCharacterMovement()->BrakingDecelerationWalking);
			DrawDebugSphere(GetWorld(), CharacterLocation + StopLocationPrediction, 10.f, 8, FColor::Green);
		}
	}
}

//~ UCLAnimInstance End
