// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/CLPlayerCharacter.h"

#include "CLGameplayTags.h"
#include "GameplayEffect.h"
#include "AbilitySystem/Attributes/CLAttributeSet.h"
#include "AbilitySystem/Attributes/CLManaAttributeSet.h"
#include "AbilitySystem/Attributes/CLStaminaAttributeSet.h"
#include "Characters/CLCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

ACLPlayerCharacter::ACLPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
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

bool ACLPlayerCharacter::CanCharacterSprint() const
{
	if (!CanMove())
	{
		UE_LOG(LogTemp, Warning, TEXT("ACLPlayerCharacter::CanCharacterSprint: Can't Move"));
		return false;
	}
	
	if (!GetCLCharacterMovement()->CanSprintInCurrentState())
	{
		UE_LOG(LogTemp, Warning, TEXT("ACLPlayerCharacter::CanCharacterSprint: Can't Sprint in Current State"));
		return false;
	}

	// Check that the player isn't fatigued
	if (HasMatchingGameplayTag(FCLGameplayTags::Get().Debuff_Fatigue))
	{
		UE_LOG(LogTemp, Warning, TEXT("ACLPlayerCharacter::CanCharacterSprint: Has Fatigue Debuff"));
		return false;
	}

	// Check that there is stamina
	if (GetStaminaAttributeSet()->GetStamina() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACLPlayerCharacter::CanCharacterSprint: No Stamina"));
		return false;
	}
	
	return true;
}

void ACLPlayerCharacter::ToggleSprinting()
{
	CastChecked<UCLCharacterMovementComponent>(GetCharacterMovement())->StartSprinting();
}

void ACLPlayerCharacter::UnToggleSprinting()
{
	CastChecked<UCLCharacterMovementComponent>(GetCharacterMovement())->StopSprinting();
}

void ACLPlayerCharacter::ApplyFatigue()
{
	UE_LOG(LogTemp, Warning, TEXT("ACLPlayerCharacter::ApplyFatigue"));
	if (HasMatchingGameplayTag(FCLGameplayTags::Get().Debuff_Fatigue))
	{
		return;
	}
	
	ApplyEffectToSelf(FatigueGameplayEffectClass, 1.f);
	if (HasMatchingGameplayTag(CLGameplayTags::Movement_CustomMode_Sprinting))
	{
		UnToggleSprinting();
	}
}

void ACLPlayerCharacter::PlayFallToRollAnimMontage()
{
	checkf(FallToRollAnimMontage, TEXT("%s uninitialized in object: %s"), GET_MEMBER_NAME_STRING_CHECKED(ACLPlayerCharacter, FallToRollAnimMontage), *GetFullName());
	if (GetAnimInstance()->Montage_Play(FallToRollAnimMontage) > 0.f)
	{
		GetAbilitySystemComponent()->SetLooseGameplayTagCount(FCLGameplayTags::Get().Locomotion_Rolling, 1);
	}
	GetAnimInstance()->Montage_SetEndDelegate(FallToRollAnimMontageEndedDelegate, FallToRollAnimMontage);
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

	FallToRollAnimMontageEndedDelegate.BindLambda(
		[this](UAnimMontage* InAnimMontage, bool bInterrupted)
		{
			GetAbilitySystemComponent()->RemoveLooseGameplayTag(FCLGameplayTags::Get().Locomotion_Rolling);
		});
}

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

	SetMovementModeTag(PrevMovementMode, PreviousCustomMode, false);
	SetMovementModeTag(GetCharacterMovement()->MovementMode, GetCharacterMovement()->CustomMovementMode, true);
}

void ACLPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsCharacterSprinting())
	{
		// If current speed is lower than regular running speed minus some delta then turn of sprinting
		if (UKismetMathLibrary::VSizeXY(GetCharacterMovement()->Velocity) < GetCharacterMovement()->MaxWalkSpeed - 0.1f)
		{
			UnToggleSprinting();
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
