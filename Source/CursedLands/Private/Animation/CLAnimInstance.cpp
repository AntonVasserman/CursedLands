// Copyright Anton Vasserman, All Rights Reserved.


#include "Animation/CLAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "Characters/CLCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCLAnimInstance::CharacterMeshSimulatePhysics() const
{
	Character->SimulatePhysics();
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

void UCLAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Character)
	{
		Velocity = MovementComponent->Velocity;
		GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);
		Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, Character->GetActorRotation());
		bShouldMove = GroundSpeed > 3.0f && MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector;
		bAlive = Character->IsAlive();
		bFalling = MovementComponent->IsFalling();
	}
}

//~ UAnimInstance End
