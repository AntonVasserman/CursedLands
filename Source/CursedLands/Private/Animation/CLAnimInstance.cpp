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

void UCLAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Character)
	{
		bAlive = Character->IsAlive();
		UpdateVelocityData();
		UpdateFallData();
	}
}

//~ UAnimInstance End
