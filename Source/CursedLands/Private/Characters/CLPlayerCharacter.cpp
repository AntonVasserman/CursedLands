// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/CLPlayerCharacter.h"

#include "CLGameplayTags.h"
#include "CLLogChannels.h"
#include "GameplayEffect.h"
#include "KismetAnimationLibrary.h"
#include "AbilitySystem/Attributes/CLAttributeSet.h"
#include "AbilitySystem/Attributes/CLManaAttributeSet.h"
#include "AbilitySystem/Attributes/CLStaminaAttributeSet.h"
#include "Characters/CLCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

ACLPlayerCharacter::ACLPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCLCharacterMovementComponent>(CharacterMovementComponentName))
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
	GetCharacterMovement()->MinAnalogWalkSpeed = MinWalkSpeed;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	GameplayCamera = CreateDefaultSubobject<UGameplayCameraComponent>("GameplayCamera");
	GameplayCamera->SetupAttachment(GetMesh());
	GameplayCamera->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	GameplayCamera->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarping");

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
	if (
		!CanMove() || bIsSprinting || // Basic check
		!GetCLCharacterMovement()->CanSprintInCurrentState() || // CMC check
		GetStaminaAttributeSet()->GetStamina() <= 0 || // Check that the PlayerCharacter has Stamina
		HasMatchingGameplayTag(CLGameplayTags::Debuff_Fatigue) // Check that the PlayerCharacter isn't fatigued
		)
	{
		return false;
	}
	
	return true;
}

void ACLPlayerCharacter::Sprint()
{
	if (CanSprint())
	{
		GetCLCharacterMovement()->bWantsToSprint = true;
	}
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	else if (!GetCLCharacterMovement()->CanEverSprint())
	{
		UE_LOG(LogCL, Warning, TEXT("%s is trying to sprint, but sprinting is disabled on this character! (check CLCharacterMovementComponent::CharacterMovementProps)"), *GetName());
	}
#endif
}

void ACLPlayerCharacter::UnSprint()
{
	GetCLCharacterMovement()->bWantsToSprint = false;
}

void ACLPlayerCharacter::ApplyFatigue()
{
	if (HasMatchingGameplayTag(CLGameplayTags::Debuff_Fatigue))
	{
		return;
	}
	
	ApplyEffectToSelf(FatigueGameplayEffectClass, 1.f);
	if (bIsSprinting)
	{
		UnSprint();
	}
}

void ACLPlayerCharacter::OnGaitChanged(ECLGait PreviousGait, ECLGait Gait)
{
	SetGaitTag(PreviousGait, false);
	SetGaitTag(Gait, true);
}

void ACLPlayerCharacter::PlayFallToRollAnimMontage()
{
	checkf(FallToRollAnimMontage, TEXT("%s uninitialized in object: %s"), GET_MEMBER_NAME_STRING_CHECKED(ACLPlayerCharacter, FallToRollAnimMontage), *GetFullName());
	if (GetAnimInstance()->Montage_Play(FallToRollAnimMontage) > 0.f)
	{
		GetAbilitySystemComponent()->SetLooseGameplayTagCount(CLGameplayTags::Locomotion_Rolling, 1);
	}
	GetAnimInstance()->Montage_SetEndDelegate(FallToRollAnimMontageEndedDelegate, FallToRollAnimMontage);
}

void ACLPlayerCharacter::PlayFallToDeathAnimMontage()
{
	checkf(FallToDeathAnimMontage, TEXT("%s uninitialized in object: %s"), GET_MEMBER_NAME_STRING_CHECKED(ACLPlayerCharacter, FallToDeathAnimMontage), *GetFullName());
	GetAnimInstance()->Montage_Play(FallToDeathAnimMontage);
	GetAnimInstance()->Montage_JumpToSection(FallToDeathAnimMontage_SectionName_Impact, FallToDeathAnimMontage);
}

void ACLPlayerCharacter::SetGaitTag(const ECLGait InGait, const bool bTagEnabled) const
{
	if (GetAbilitySystemComponent())
	{
		if (const FGameplayTag* GaitTag = CLGameplayTags::GaitTagMap.Find(InGait);
			GaitTag && GaitTag->IsValid())
		{
			GetAbilitySystemComponent()->SetLooseGameplayTagCount(*GaitTag, bTagEnabled ? 1 : 0);
		}
	}
}

void ACLPlayerCharacter::UpdateCardinalDirectionAngle()
{
	const FRotator PlayerCharacterRotation = GetActorRotation();
	const FVector PlayerCharacterVelocity2D = GetVelocity() * FVector(1, 1, 0);
	CardinalDirectionAngle = UKismetAnimationLibrary::CalculateDirection(PlayerCharacterVelocity2D, PlayerCharacterRotation);
}

void ACLPlayerCharacter::UpdateCardinalDirection()
{
	// Handling Deadzone first
	bool bIsInDeadzone = false;
	switch (CardinalDirection)
	{
	case ECLCardinalDirection::Forward:
		bIsInDeadzone = CardinalDirectionAngle >= CardinalDirectionForwardMin - CardinalDirectionDeadzone && CardinalDirectionAngle <= CardinalDirectionForwardMax + CardinalDirectionDeadzone; 
		break;
	case ECLCardinalDirection::Backward:
		bIsInDeadzone = CardinalDirectionAngle < CardinalDirectionBackwardMin + CardinalDirectionDeadzone || CardinalDirectionAngle > CardinalDirectionBackwardMax - CardinalDirectionDeadzone; 
		break;
	case ECLCardinalDirection::Right:
		bIsInDeadzone = CardinalDirectionAngle > CardinalDirectionForwardMax - CardinalDirectionDeadzone && CardinalDirectionAngle < CardinalDirectionBackwardMax + CardinalDirectionDeadzone;
		break;
	case ECLCardinalDirection::Left:
		bIsInDeadzone = CardinalDirectionAngle > CardinalDirectionBackwardMin - CardinalDirectionDeadzone && CardinalDirectionAngle < CardinalDirectionForwardMin + CardinalDirectionDeadzone;
		break;
	default:
		checkNoEntry();
	}

	if (bIsInDeadzone)
	{
		return;
	}
	
	// Evaluate Direction outside of deadzone
	if (CardinalDirectionAngle >= CardinalDirectionForwardMin && CardinalDirectionAngle <= CardinalDirectionForwardMax) // Forward Direction
	{
		CardinalDirection = ECLCardinalDirection::Forward;
	}
	else if (CardinalDirectionAngle < CardinalDirectionBackwardMin || CardinalDirectionAngle > CardinalDirectionBackwardMax) // Backward Direction
	{
		CardinalDirection = ECLCardinalDirection::Backward;
	}
	else if (CardinalDirectionAngle > 0) // Right Direction
	{
		CardinalDirection = ECLCardinalDirection::Right;
	}
	else
	{
		CardinalDirection = ECLCardinalDirection::Left;
	}
}

//~ ACLCharacter Begin

void ACLPlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (const float FallHeight = GetCLCharacterMovement()->GetFallHeight();
		FallHeight >= FallHeightForMinFallDamage)
	{
		checkf(FallDamageGameplayEffectClass, TEXT("FallDamageGameplayEffectClass uninitialized in object: %s"), *GetFullName());
		const float NormalizedFallHeight = UKismetMathLibrary::NormalizeToRange(FallHeight, FallHeightForMinFallDamage, FallHeightForMaxFallDamage);
		const float FallDamageLevel = FMath::Clamp(NormalizedFallHeight, 0.f, 1.f);

		ApplyEffectToSelf(FallDamageGameplayEffectClass, FallDamageLevel);

		if (FallHeight < FallHeightForMaxFallDamage)
		{
			// We play the fall to roll animation even if falling results in death because the FallToRollAnimMontage is responsible for simulating
			// physics upon death.
			OnFellToRoll.Broadcast();
			PlayFallToRollAnimMontage();
		}
		else
		{
			OnFellToDeath.Broadcast();
			PlayFallToDeathAnimMontage();
		}
	}
}

void ACLPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GetCLCharacterMovement()->OnGaitChanged.AddDynamic(this, &ACLPlayerCharacter::OnGaitChanged);
	
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(GetStaminaAttributeSet()->GetStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			if (Data.NewValue == 0)
			{
				ApplyFatigue();
			}
		});
	
	FallToRollAnimMontageEndedDelegate.BindLambda(
		[this](UAnimMontage* InAnimMontage, bool bInterrupted)
		{
			GetAbilitySystemComponent()->RemoveLooseGameplayTag(CLGameplayTags::Locomotion_Rolling);
		});
}

void ACLPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateCardinalDirectionAngle();
	UpdateCardinalDirection();
	
	if (bIsSprinting)
	{
		// If current speed is lower than regular running speed minus some delta then turn of sprinting
		if (UKismetMathLibrary::VSizeXY(GetCharacterMovement()->Velocity) < GetCLCharacterMovement()->GetGaitSettings(ECLGait::Jogging).MaxWalkingSpeed - 0.1f)
		{
			UnSprint();
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
