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
		bAlive = Character->IsAlive();
		UpdateVelocityData();
		UpdateFallData();
		UpdateLocationData(DeltaSeconds);
	}

	bFirstThreadSafeUpdate = false;
}

//~ UAnimInstance End
