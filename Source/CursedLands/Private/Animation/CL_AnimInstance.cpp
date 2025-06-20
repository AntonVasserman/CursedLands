// Copyright Anton Vasserman, All Rights Reserved.


#include "Animation/CL_AnimInstance.h"

#include "Characters/CL_Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCL_AnimInstance::UpdateLocationData(const float DeltaTime)
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

void UCL_AnimInstance::UpdateVelocityData()
{
	Velocity = MovementComponent->Velocity;
	Velocity2D = FVector(Velocity.X, Velocity.Y, 0.f);
	Velocity2DSize = Velocity2D.Size();
}

void UCL_AnimInstance::UpdateFallData()
{
	bFalling = MovementComponent->IsFalling();
}

//~ UAnimInstance Begin

void UCL_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (ACL_Character* OwningCharacter = Cast<ACL_Character>(GetOwningActor()))
	{
		Character = OwningCharacter;
		MovementComponent = Character->GetCharacterMovement();
	}
}

void UCL_AnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (Character)
	{
// We don't want to run this code in the editor because it relies on the GAS Component.
// The GAS Component is initialized only on BeginPlay so we shouldn't do this code in the Editor.
#ifndef WITH_EDITOR
		bAlive = Character->IsAlive();
#endif
		UpdateVelocityData();
		UpdateFallData();
		UpdateLocationData(DeltaSeconds);
	}

	bFirstThreadSafeUpdate = false;
}

//~ UAnimInstance End
