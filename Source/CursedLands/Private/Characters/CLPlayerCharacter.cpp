// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/CLPlayerCharacter.h"

#include "CLGameplayTags.h"
#include "GameplayEffect.h"
#include "AbilitySystem/Attributes/CLAttributeSet.h"
#include "AbilitySystem/Attributes/CLManaAttributeSet.h"
#include "AbilitySystem/Attributes/CLStaminaAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

ACLPlayerCharacter::ACLPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = MinWalkSpeed;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	GameplayCamera = CreateDefaultSubobject<UGameplayCameraComponent>("GameplayCamera");
	GameplayCamera->SetupAttachment(GetMesh());

	ManaAttributeSet = CreateDefaultSubobject<UCLManaAttributeSet>("ManaAttributeSet");
	StaminaAttributeSet = CreateDefaultSubobject<UCLStaminaAttributeSet>("StaminaAttributeSet");
}

bool ACLPlayerCharacter::CanSprint() const
{
	UE_LOG(LogTemp, Warning, TEXT("ACLPlayerCharacter::CanSprint"));
	// Check that the player isn't fatigued
	if (HasMatchingGameplayTag(FCLGameplayTags::Get().Debuff_Fatigue))
	{
		UE_LOG(LogTemp, Warning, TEXT("ACLPlayerCharacter::CanSprint: Has Fatigue Debuff"));
		return false;
	}

	// Check that there is stamina
	if (GetStaminaAttributeSet()->GetStamina() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACLPlayerCharacter::CanSprint: No Stamina"));
		return false;
	}
	
	return true;
}

void ACLPlayerCharacter::ToggleSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	AddUniqueGameplayTag(FCLGameplayTags::Get().Locomotion_Sprinting);
}

void ACLPlayerCharacter::UnToggleSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	RemoveGameplayTag(FCLGameplayTags::Get().Locomotion_Sprinting);
}

void ACLPlayerCharacter::ApplyFatigue()
{
	UE_LOG(LogTemp, Warning, TEXT("ACLPlayerCharacter::ApplyFatigue"));
	if (HasMatchingGameplayTag(FCLGameplayTags::Get().Debuff_Fatigue))
	{
		return;
	}
	
	ApplyEffectToSelf(FatigueGameplayEffectClass, 1.f);
	if (HasMatchingGameplayTag(FCLGameplayTags::Get().Locomotion_Sprinting))
	{
		UnToggleSprint();
	}
}

//~ ACLCharacter Begin

void ACLPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(GetStaminaAttributeSet()->GetStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			if (Data.NewValue == 0)
			{
				ApplyFatigue();
			}
		});
}

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

void ACLPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Check if character is sprinting
	if (GetAbilitySystemComponent()->HasMatchingGameplayTag(FCLGameplayTags::Get().Locomotion_Sprinting))
	{
		// If current speed is lower than regular running speed minus some delta then turn of sprinting
		if (UKismetMathLibrary::VSizeXY(GetCharacterMovement()->Velocity) < RunSpeed - 0.1f)
		{
			UnToggleSprint();
		}
	}
}

//~ ACLCharacter End