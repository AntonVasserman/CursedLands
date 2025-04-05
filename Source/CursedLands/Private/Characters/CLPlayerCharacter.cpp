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
	GameplayCamera->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	GameplayCamera->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	ManaAttributeSet = CreateDefaultSubobject<UCLManaAttributeSet>("ManaAttributeSet");
	StaminaAttributeSet = CreateDefaultSubobject<UCLStaminaAttributeSet>("StaminaAttributeSet");
}

void ACLPlayerCharacter::SetMovementMode(const ECLPlayerCharacterMovementMode InMovementMode)
{
	MovementMode = InMovementMode;
	switch (MovementMode)
	{
	case ECLPlayerCharacterMovementMode::Default:
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		break;
	case ECLPlayerCharacterMovementMode::Strafing:
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		break;
	}
}

bool ACLPlayerCharacter::CanSprint() const
{
	if (!CanMove())
	{
		UE_LOG(LogTemp, Warning, TEXT("ACLPlayerCharacter::CanSprint: Can't Move"));
		return false;
	}
	
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

// TODO: Consider improving this function to return a CLPlayerAnimInstance?
UAnimInstance* ACLPlayerCharacter::GetAnimInstance()
{
	return GetMesh()->GetAnimInstance();
}

void ACLPlayerCharacter::PlayFallToRollAnimMontage()
{
	checkf(FallToRollAnimMontage, TEXT("%s uninitialized in object: %s"), GET_MEMBER_NAME_STRING_CHECKED(ACLPlayerCharacter, FallToRollAnimMontage), *GetFullName());
	GetAnimInstance()->Montage_Play(FallToRollAnimMontage);
}

void ACLPlayerCharacter::PlayFallToDeathAnimMontage()
{
	checkf(FallToDeathAnimMontage, TEXT("%s uninitialized in object: %s"), GET_MEMBER_NAME_STRING_CHECKED(ACLPlayerCharacter, FallToDeathAnimMontage), *GetFullName());
	GetAnimInstance()->Montage_Play(FallToDeathAnimMontage);
	GetAnimInstance()->Montage_JumpToSection(FallToDeathAnimMontage_SectionName_Impact, FallToDeathAnimMontage);
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

	if (FallHeight >= FallHeightForMinFallDamage)
	{
		checkf(FallDamageGameplayEffectClass, TEXT("FallDamageGameplayEffectClass uninitialized in object: %s"), *GetFullName());
		const float NormalizedFallHeight = UKismetMathLibrary::NormalizeToRange(FallHeight, FallHeightForMinFallDamage, FallHeightForMaxFallDamage);
		const float FallDamageLevel = FMath::Clamp(NormalizedFallHeight, 0.f, 1.f);

		ApplyEffectToSelf(FallDamageGameplayEffectClass, FallDamageLevel);

		// TODO: add IsAlive() here
		if (FallHeight < FallHeightForMaxFallDamage)
		{
			PlayFallToRollAnimMontage();
		}
		else
		{
			PlayFallToDeathAnimMontage();
		}
	}
}

void ACLPlayerCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	// If we previously weren't falling, and now we do, it means we just started falling
	if (PrevMovementMode != MOVE_Falling && GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		FallBeginZ = GetActorLocation().Z;
	}
}

void ACLPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// We want to always evaluate FallHeight as we use it for multiple falling animations 
	if (GetCharacterMovement()->IsFalling())
	{
		FallHeight = FallBeginZ - GetActorLocation().Z;
	}
	
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

void ACLPlayerCharacter::Die()
{
	// In the case of death we would like to first execute our logic and only then the rest of the parent's logic
	GetGameplayCamera()->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
	
	Super::Die();
}

//~ ACLCharacter End
