// Copyright Anton Vasserman, All Rights Reserved.


#include "Animation/CLPlayerCharacterAnimInstance.h"

#include "Characters/CLPlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

static TAutoConsoleVariable CVarShowDebugCLPlayerAnimInstance(
	TEXT("CLShowDebug.PlayerCharacterAnimInstance"),
	false,
	TEXT("Shows the Debug information of the CLPlayerAnimInstance class"),
	ECVF_Default);

void UCLPlayerCharacterAnimInstance::UpdateLocomotionData(const ACLPlayerCharacter* InPlayerCharacter)
{
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
	
	MovementMode = PlayerCharacter->GetMovementMode();
	FallHeight = PlayerCharacter->GetCLCharacterMovement()->GetFallHeight();

	UpdateLocomotionData(PlayerCharacter);
	UpdateRotationData(DeltaSeconds, PlayerCharacter);

	if (CVarShowDebugCLPlayerAnimInstance->GetBool() && GEngine)
	{
		const FColor TextColor = FColor::Red;
		const FVector2D TextScale = FVector2D(1.5f, 1.5f);

		// Putting them in reverse order since the first added is actually last on screen
		GEngine->AddOnScreenDebugMessage(11, 0.0f, TextColor, FString::Printf(TEXT("LeanAngle: %f"), LeanAngle), false, TextScale);
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

		// Draw Direction/Velocity
		const FVector PlayerCharacterLocation = PlayerCharacter->GetActorLocation();
		const FVector DirectionalArrowLineStart = FVector(PlayerCharacterLocation.X, PlayerCharacterLocation.Y, PlayerCharacterLocation.Z - 60.f);
		const FVector DirectionalArrowLineEnd = DirectionalArrowLineStart + Velocity;
		DrawDebugDirectionalArrow(GetWorld(), DirectionalArrowLineStart, DirectionalArrowLineEnd, 5.f, FColor::Red, false, -1, 0, 2.5f);
	}

	bFirstUpdate = false;
}

//~ UCLAnimInstance End
