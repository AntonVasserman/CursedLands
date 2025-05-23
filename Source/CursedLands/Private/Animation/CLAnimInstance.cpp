// Copyright Anton Vasserman, All Rights Reserved.


#include "Animation/CLAnimInstance.h"

#include "Characters/CLCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCLAnimInstance::UpdateLocationData(const float DeltaTime)
{
	LastCharacterLocation = CharacterLocation;
	CharacterLocation = Character->GetActorLocation();
	CharacterLocationDeltaSizeXY = UKismetMathLibrary::VSizeXY(CharacterLocation - LastCharacterLocation);
	CharacterLocationDeltaSizeXYSpeed = CharacterLocationDeltaSizeXY / DeltaTime;
	
	if (bFirstThreadSafeUpdate)
	{
		LastCharacterLocation = FVector::ZeroVector;
		CharacterLocationDeltaSizeXY = 0.f;
		CharacterLocationDeltaSizeXYSpeed = 0.f;
	}
}

void UCLAnimInstance::UpdateVelocityData()
{
	Velocity = MovementComponent->Velocity;
	Velocity2D = FVector(Velocity.X, Velocity.Y, 0.f);
	Velocity2DSize = Velocity2D.Size();
}

void UCLAnimInstance::UpdateFallData()
{
	bFalling = MovementComponent->IsFalling();
}

//~ UAnimInstance Begin

void UCLAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (ACLCharacter* OwningCharacter = Cast<ACLCharacter>(GetOwningActor()))
	{
		Character = OwningCharacter;
		MovementComponent = Character->GetCharacterMovement();
	}
}

void UCLAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (Character)
	{
		bAlive = Character->IsAlive();
		UpdateVelocityData();
		UpdateFallData();
		UpdateLocationData(DeltaSeconds);
	}

	bFirstThreadSafeUpdate = false;
}

//~ UAnimInstance End
