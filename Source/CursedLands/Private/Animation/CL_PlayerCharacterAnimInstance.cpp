// Copyright Anton Vasserman, All Rights Reserved.


#include "Animation/CL_PlayerCharacterAnimInstance.h"

#include "AnimCharacterMovementLibrary.h"
#include "AV_DrawDebugStatics.h"
#include "KismetAnimationLibrary.h"
#include "Characters/CL_PlayerCharacter.h"
#include "Components/AV_CharacterFallComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Systems/CharacterFall/CL_FallType.h"
#include "Systems/Traversal/CL_CharacterTraversalComponent.h"

static TAutoConsoleVariable CVarShowDebugCLPlayerAnimInstance(
	TEXT("CLShowDebug.PlayerCharacterAnimInstance"),
	false,
	TEXT("Shows the Debug information of the CLPlayerAnimInstance class"),
	ECVF_Default);

//~ UCLAnimInstance Begin

void UCL_PlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// Await threadsafe update that populates most of the data
	if (!bFirstThreadSafeUpdate)
	{
		if (
			// !bFirstThreadSafeUpdate &&
			CVarShowDebugCLPlayerAnimInstance->GetBool() &&
			GetWorld() && GetWorld()->IsPlayInEditor() &&
			GEngine
			)
		{
			const FColor TextColor = FColor::Red;
			const FVector2D TextScale = FVector2D(1.5f, 1.5f);

			// Putting them in reverse order since the first added is actually last on screen
			// GEngine->AddOnScreenDebugMessage(71, 0.0f, TextColor, FString::Printf(TEXT("Root Yaw Offset Data::RootYawOffsetMode: %s"), *StaticEnum<ECL_RootYawOffsetMode>()->GetAuthoredNameStringByValue(static_cast<int64>(RootYawOffsetMode))), false, TextScale);
			// GEngine->AddOnScreenDebugMessage(70, 0.f, TextColor, FString::Printf(TEXT("Root Yaw Offset Data::RootYawOffset: %f"), RootYawOffset), false, TextScale);
			
			// GEngine->AddOnScreenDebugMessage(60, 0.0f, TextColor, FString::Printf(TEXT("Rotation Data::LeanAngle: %f"), LeanAngle), false, TextScale);

			// GEngine->AddOnScreenDebugMessage(51, 0.0f, TextColor, FString::Printf(TEXT("Locomotion SM Data::PivotCardinalDirection: %s"), *StaticEnum<ECL_CardinalDirection>()->GetAuthoredNameStringByValue(static_cast<int64>(PivotCardinalDirection))), false, TextScale);
			// GEngine->AddOnScreenDebugMessage(50, 0.0f, TextColor, FString::Printf(TEXT("Locomotion SM Data::PivotAcceleration2D: %s"), *PivotAcceleration2D.ToString()), false, TextScale);

			// GEngine->AddOnScreenDebugMessage(44, 0.0f, TextColor, FString::Printf(TEXT("Locomotion Data::Stance: %s"), *StaticEnum<ECL_Stance>()->GetAuthoredNameStringByValue(static_cast<int64>(Stance))), false, TextScale);
			// GEngine->AddOnScreenDebugMessage(43, 0.0f, TextColor, FString::Printf(TEXT("Locomotion Data::Gait: %s"), *StaticEnum<ECL_Gait>()->GetAuthoredNameStringByValue(static_cast<int64>(Gait))), false, TextScale);
			// GEngine->AddOnScreenDebugMessage(42, 0.0f, TextColor, FString::Printf(TEXT("Locomotion Data::CardinalDirectionAngle: %f"), CardinalDirectionAngle), false, TextScale);
			// GEngine->AddOnScreenDebugMessage(41, 0.0f, TextColor, FString::Printf(TEXT("Locomotion Data::CardinalDirection: %s"), *StaticEnum<ECL_CardinalDirection>()->GetAuthoredNameStringByValue(static_cast<int64>(CardinalDirection))), false, TextScale);
			// GEngine->AddOnScreenDebugMessage(40, 0.0f, TextColor, FString::Printf(TEXT("Locomotion Data::MovementMode: %s"), *StaticEnum<ECL_PlayerCharacterMovementMode>()->GetAuthoredNameStringByValue(static_cast<int64>(MovementMode))), false, TextScale);

			// GEngine->AddOnScreenDebugMessage(32, 0.0f, TextColor, FString::Printf(TEXT("Acceleration Data::AccelerationAngle: %f"), AccelerationAngle), false, TextScale);
			// GEngine->AddOnScreenDebugMessage(31, 0.0f, TextColor, FString::Printf(TEXT("Acceleration Data::Accelerating: %d"), bAccelerating), false, TextScale);
			// GEngine->AddOnScreenDebugMessage(30, 0.0f, TextColor, FString::Printf(TEXT("Acceleration Data::Acceleration: %s"), *Acceleration.ToString()), false, TextScale);

			// GEngine->AddOnScreenDebugMessage(22, 0.0f, TextColor, FString::Printf(TEXT("Fall Data::FallType: %s"), *StaticEnum<ECL_FallType>()->GetAuthoredNameStringByValue(static_cast<int64>(FallType))), false, TextScale);
			// GEngine->AddOnScreenDebugMessage(21, 0.0f, TextColor, FString::Printf(TEXT("Fall Data::FallHeight: %f"), FallHeight), false, TextScale);
			// GEngine->AddOnScreenDebugMessage(20, 0.0f, TextColor, FString::Printf(TEXT("Fall Data::Falling: %d"), bFalling), false, TextScale);

			// GEngine->AddOnScreenDebugMessage(11, 0.0f, TextColor, FString::Printf(TEXT("Velocity Data::Velocity2DSize: %f"), Velocity2DSize), false, TextScale);
			// GEngine->AddOnScreenDebugMessage(10, 0.0f, TextColor, FString::Printf(TEXT("Velocity Data::Velocity: %s"), *Velocity.ToString()), false, TextScale);

			// GEngine->AddOnScreenDebugMessage(1, 0.0f, TextColor, FString::Printf(TEXT("Location Data::CharacterLocationDeltaSizeXY: %f"), CharacterLocationDeltaSizeXY), false, TextScale);
			// GEngine->AddOnScreenDebugMessage(0, 0.0f, TextColor, FString::Printf(TEXT("Location Data::CharacterLocation: %s"), *CharacterLocation.ToString()), false, TextScale);

			// // Draw Direction/Velocity Vector
			// const FVector DebugVectorStart = FVector(CharacterLocation.X, CharacterLocation.Y, CharacterLocation.Z - 60.f);
			// const FVector VelocityDebugVectorEnd = DebugVectorStart + Velocity;
			// UAV_DrawDebugStatics::DrawDebugNamedDirectionalArrow(GetWorld(), DebugVectorStart, VelocityDebugVectorEnd, FString(TEXT("Velocity")), FColor::Green, 200.f);
			//
			// // Draw Acceleration Vector
			// const FVector AccelerationDebugVectorEnd = DebugVectorStart + Acceleration;
			// UAV_DrawDebugStatics::DrawDebugNamedDirectionalArrow(GetWorld(), DebugVectorStart, AccelerationDebugVectorEnd, FString(TEXT("Acceleration")), FColor::Yellow, 200.f);
			//
			// // Draw Predicted Ground Movement Stop Location
			// const FVector StopLocationPrediction = UAnimCharacterMovementLibrary::PredictGroundMovementStopLocation(
			// 	Velocity2D,
			// 	PlayerCharacter->GetCharacterMovement()->bUseSeparateBrakingFriction,
			// 	PlayerCharacter->GetCharacterMovement()->BrakingFriction,
			// 	PlayerCharacter->GetCharacterMovement()->GroundFriction,
			// 	PlayerCharacter->GetCharacterMovement()->BrakingFrictionFactor,
			// 	PlayerCharacter->GetCharacterMovement()->BrakingDecelerationWalking);
			// DrawDebugSphere(GetWorld(), CharacterLocation + StopLocationPrediction, 10.f, 8, FColor::Green);
		}
	}
}

//~ UCLAnimInstance End
