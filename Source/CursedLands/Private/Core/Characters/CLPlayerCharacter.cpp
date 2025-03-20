// Copyright Anton Vasserman, All Rights Reserved.


#include "Core/Characters/CLPlayerCharacter.h"

#include "GameplayEffect.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

ACLPlayerCharacter::ACLPlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	GameplayCamera = CreateDefaultSubobject<UGameplayCameraComponent>("GameplayCamera");
	GameplayCamera->SetupAttachment(GetMesh());
}

//~ ACLCharacter Begin

void ACLPlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	// We don't do a checkf() here because we might not want to apply any FallDamage in the game, so setting the effect isn't mandatory
	if (FallDamageGameplayEffectClass)
	{
		const float ZVelocity = GetVelocity().Z;
		const float AbsZVelocity = FMath::Abs(ZVelocity);
		const float NormalizedZVelocity = UKismetMathLibrary::NormalizeToRange(AbsZVelocity, VelocityForMinFallDamage, VelocityForMaxFallDamage);
		const float FallDamageLevel = FMath::Clamp(NormalizedZVelocity, 0.0f, 1.0f);
		ApplyEffectToSelf(FallDamageGameplayEffectClass, FallDamageLevel);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("ACLPlayerCharacter::Landed: FallDamageGameplayEffectClass isn't set so no damage applied on falling"));
	}
}

//~ ACLCharacter End