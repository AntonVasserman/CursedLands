// Copyright Anton Vasserman, All Rights Reserved.


#include "Animation/CLPlayerCharacterAnimInstance.h"

#include "CLLogChannels.h"
#include "Characters/CLPlayerCharacter.h"

static TAutoConsoleVariable CVarShowDebugCLPlayerAnimInstance(
	TEXT("ShowDebug.CLPlayerAnimInstance"),
	false,
	TEXT("Shows the Debug information of the CLPlayerAnimInstance class"),
	ECVF_Default);

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

	if (PlayerCharacter)
	{
		MovementMode = PlayerCharacter->GetMovementMode();
		FallHeight = PlayerCharacter->GetCLCharacterMovement()->GetFallHeight();
	}

	if (CVarShowDebugCLPlayerAnimInstance->GetBool() && GEngine)
	{
		const FColor TextColor = FColor::Red;
		const FVector2D TextScale = FVector2D(1.5f, 1.5f);

		// Putting them in reverse order since the first added is actually last on screen
		GEngine->AddOnScreenDebugMessage(0, 0.0f, TextColor, FString::Printf(TEXT("FallHeight: %f"), FallHeight), false, TextScale);
		GEngine->AddOnScreenDebugMessage(1, 0.0f, TextColor, FString::Printf(TEXT("MovementMode: %s"), *StaticEnum<ECLPlayerCharacterMovementMode>()->GetAuthoredNameStringByValue(static_cast<int64>(MovementMode))), false, TextScale);
		
		GEngine->AddOnScreenDebugMessage(2, 0.0f, TextColor, FString::Printf(TEXT("bFalling: %d"), bFalling), false, TextScale);
		GEngine->AddOnScreenDebugMessage(3, 0.0f, TextColor, FString::Printf(TEXT("bAlive: %d"), bAlive), false, TextScale);
		GEngine->AddOnScreenDebugMessage(4, 0.0f, TextColor, FString::Printf(TEXT("bShouldMove: %d"), bShouldMove), false, TextScale);
		GEngine->AddOnScreenDebugMessage(5, 0.0f, TextColor, FString::Printf(TEXT("Direction: %f"), Direction), false, TextScale);
		GEngine->AddOnScreenDebugMessage(6, 0.0f, TextColor, FString::Printf(TEXT("GroundSpeed: %f"), GroundSpeed), false, TextScale);
		GEngine->AddOnScreenDebugMessage(7, 0.0f, TextColor, FString::Printf(TEXT("Velocity: %s"), *Velocity.ToString()), false, TextScale);
	}
}

//~ UCLAnimInstance End
