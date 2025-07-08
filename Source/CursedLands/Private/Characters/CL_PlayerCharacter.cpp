// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/CL_PlayerCharacter.h"

#include "CL_GameplayTags.h"
#include "KismetAnimationLibrary.h"
#include "AbilitySystem/CL_AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/CL_AttributeSet.h"
#include "AbilitySystem/Components/CL_ManaComponent.h"
#include "AbilitySystem/Components/CL_StaminaComponent.h"
#include "Characters/Components/CL_ExtendedCharacterMovementComponent.h"
#include "Components/AV_CharacterFallComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Systems/Traversal/CL_CharacterTraversalComponent.h"

DEFINE_LOG_CATEGORY(LogCLPlayerCharacter);

ACL_PlayerCharacter::ACL_PlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCL_ExtendedCharacterMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->InitCapsuleSize(40.f, 90.0f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MinAnalogWalkSpeed = MinWalkSpeed;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->SetCrouchedHalfHeight(65.f);

	GameplayCamera = CreateDefaultSubobject<UGameplayCameraComponent>("GameplayCamera");
	GameplayCamera->SetupAttachment(GetMesh());
	GameplayCamera->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	
	CharacterFallComponent = CreateDefaultSubobject<UAV_CharacterFallComponent>("CharacterFall");
	
	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarping");
	CharacterTraversal = CreateDefaultSubobject<UCL_CharacterTraversalComponent>("CharacterTraversal");

	ManaComponent = CreateDefaultSubobject<UCL_ManaComponent>("ManaComponent");
	StaminaComponent = CreateDefaultSubobject<UCL_StaminaComponent>("StaminaComponent");
}

void ACL_PlayerCharacter::SetMovementMode(const ECL_PlayerCharacterMovementMode InMovementMode)
{
	MovementMode = InMovementMode;
	switch (MovementMode)
	{
	case ECL_PlayerCharacterMovementMode::Default:
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		break;
	case ECL_PlayerCharacterMovementMode::Strafing:
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		break;
	}
}

bool ACL_PlayerCharacter::CanWalk() const
{
	if (
		!CanMove() || !IsStanding() || IsWalking() || // Basic check
		GetCharacterTraversal()->IsDoingTraversalAction() || // If player is occupied (traversing) it can't walk
		!GetCLCharacterMovement()->CanWalkInCurrentState() // CMC check
		)
	{
		return false;
	}

	return true;
}

void ACL_PlayerCharacter::Walk()
{
	if (CanWalk())
	{
		GetCLCharacterMovement()->RequestWalking();
	}
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	else if (!GetCLCharacterMovement()->CanEverWalk())
	{
		UE_LOG(LogCLPlayerCharacter, Warning, TEXT("%s is trying to walk, but walking is disabled on this character! (check CLCharacterMovementComponent::CharacterMovementProps)"), *GetName());
	}
#endif
}

void ACL_PlayerCharacter::UnWalk()
{
	GetCLCharacterMovement()->RequestJogging();
}

bool ACL_PlayerCharacter::CanSprint() const
{
	if (
		!CanMove() || !IsStanding() || IsSprinting() || // Basic check
		GetCharacterTraversal()->IsDoingTraversalAction() || // If the player is occupied (traversing) then it can't sprint
		!GetCLCharacterMovement()->CanSprintInCurrentState() || // CMC check
		GetStaminaComponent()->GetValue() <= 0 || // Check that the PlayerCharacter has Stamina
		StaminaComponent->IsFatigued()
		)
	{
		return false;
	}
	
	return true;
}

void ACL_PlayerCharacter::Sprint()
{
	if (CanSprint())
	{
		GetCLCharacterMovement()->RequestSprinting();
	}
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	else if (!GetCLCharacterMovement()->CanEverSprint())
	{
		UE_LOG(LogCLPlayerCharacter, Warning, TEXT("%s is trying to sprint, but sprinting is disabled on this character! (check CLCharacterMovementComponent::CharacterMovementProps)"), *GetName());
	}
#endif
}

void ACL_PlayerCharacter::UnSprint()
{
	GetCLCharacterMovement()->RequestUnSprinting();
}

bool ACL_PlayerCharacter::CanTraverse() const
{
	return CharacterTraversal->CanDoTraversalAction();
}

void ACL_PlayerCharacter::Traverse()
{
	if (CharacterTraversal->CanDoTraversalAction())
	{
		CharacterTraversal->RequestTraversalAction();
	}
}

bool ACL_PlayerCharacter::CanSlide() const
{
	// We allow Sliding only when sprinting
	return CharacterTraversal->CanDoTraversalAction() && IsSprinting();
}

void ACL_PlayerCharacter::Slide()
{
	if (CanSlide())
	{
		CharacterTraversal->RequestSlidingAction();
	}
}

void ACL_PlayerCharacter::FatigueApplied()
{
	if (IsSprinting())
	{
		UnSprint();
	}
}

void ACL_PlayerCharacter::OnStanceChanged(const ECL_Stance PreviousStance, const ECL_Stance Stance)
{
	SetStanceTag(PreviousStance, false);
	SetStanceTag(Stance, true);
}

void ACL_PlayerCharacter::OnGaitChanged(const ECL_Gait PreviousGait, const ECL_Gait Gait)
{
	SetGaitTag(PreviousGait, false);
	SetGaitTag(Gait, true);

	if (PreviousGait == ECL_Gait::Sprinting && Gait != ECL_Gait::Sprinting)
	{
		bFullySprinting = false;
	}
}

void ACL_PlayerCharacter::SetStanceTag(const ECL_Stance InStance, const bool bTagEnabled) const
{
	if (GetAbilitySystemComponent())
	{
		if (const FGameplayTag* StanceTag = CLGameplayTags::StanceTagMap.Find(InStance);
			StanceTag && StanceTag->IsValid())
		{
			GetAbilitySystemComponent()->SetLooseGameplayTagCount(*StanceTag, bTagEnabled ? 1 : 0);
		}
	}
}

void ACL_PlayerCharacter::SetGaitTag(const ECL_Gait InGait, const bool bTagEnabled) const
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

void ACL_PlayerCharacter::SetTraversalActionTag(const ECL_TraversalAction InTraversalAction, const bool bTagEnabled) const
{
	if (GetAbilitySystemComponent())
	{
		if (const FGameplayTag* TraversalActionTag = CLGameplayTags::TraversalActionTagMap.Find(InTraversalAction);
			TraversalActionTag && TraversalActionTag->IsValid())
		{
			GetAbilitySystemComponent()->SetLooseGameplayTagCount(*TraversalActionTag, bTagEnabled ? 1 : 0);
		}
	}
}

void ACL_PlayerCharacter::UpdateCardinalDirectionAngle()
{
	const FRotator PlayerCharacterRotation = GetActorRotation();
	const FVector PlayerCharacterVelocity2D = GetVelocity() * FVector(1, 1, 0);
	CardinalDirectionAngle = UKismetAnimationLibrary::CalculateDirection(PlayerCharacterVelocity2D, PlayerCharacterRotation);
}

void ACL_PlayerCharacter::UpdateCardinalDirection()
{
	// Handling Deadzone first
	bool bIsInDeadzone = false;
	switch (CardinalDirection)
	{
	case ECL_CardinalDirection::Forward:
		bIsInDeadzone = CardinalDirectionAngle >= CardinalDirectionForwardMin - CardinalDirectionDeadzone && CardinalDirectionAngle <= CardinalDirectionForwardMax + CardinalDirectionDeadzone; 
		break;
	case ECL_CardinalDirection::Backward:
		bIsInDeadzone = CardinalDirectionAngle < CardinalDirectionBackwardMin + CardinalDirectionDeadzone || CardinalDirectionAngle > CardinalDirectionBackwardMax - CardinalDirectionDeadzone; 
		break;
	case ECL_CardinalDirection::Right:
		bIsInDeadzone = CardinalDirectionAngle > CardinalDirectionForwardMax - CardinalDirectionDeadzone && CardinalDirectionAngle < CardinalDirectionBackwardMax + CardinalDirectionDeadzone;
		break;
	case ECL_CardinalDirection::Left:
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
		CardinalDirection = ECL_CardinalDirection::Forward;
	}
	else if (CardinalDirectionAngle < CardinalDirectionBackwardMin || CardinalDirectionAngle > CardinalDirectionBackwardMax) // Backward Direction
	{
		CardinalDirection = ECL_CardinalDirection::Backward;
	}
	else if (CardinalDirectionAngle > 0) // Right Direction
	{
		CardinalDirection = ECL_CardinalDirection::Right;
	}
	else
	{
		CardinalDirection = ECL_CardinalDirection::Left;
	}
}

void ACL_PlayerCharacter::OnCharacterTraversalActionStarted(const ECL_TraversalAction TraversalAction)
{
	UE_LOG(LogCLPlayerCharacter, Display, TEXT("%hs: Started Traversal Action '%s'"), __FUNCTION__, *StaticEnum<ECL_TraversalAction>()->GetAuthoredNameStringByValue(static_cast<int64>(TraversalAction)));

	SetTraversalActionTag(TraversalAction, true);
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Since CharacterMovementComponent might reset sprinting when we are "Flying" we need to be able to request sprinting once traversal is over
	bSprintAfterTraversal = IsSprinting();
}

void ACL_PlayerCharacter::OnCharacterTraversalActionFinished(const ECL_TraversalAction TraversalAction)
{
	UE_LOG(LogCLPlayerCharacter, Display, TEXT("%hs: Finished Traversal Action '%s'"), __FUNCTION__, *StaticEnum<ECL_TraversalAction>()->GetAuthoredNameStringByValue(static_cast<int64>(TraversalAction)));

	SetTraversalActionTag(TraversalAction, false);
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// In case we were sprinting before traversal, return sprinting
	if (bSprintAfterTraversal)
	{
		Sprint();
		bSprintAfterTraversal = false;
	}
}

void ACL_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Setup Mana Component Initialization
	GetManaComponent()->InitializeWithAbilitySystem(GetCLAbilitySystemComponent());
	
	// Setup Stamina Component Initialization
	GetStaminaComponent()->InitializeWithAbilitySystem(GetCLAbilitySystemComponent());
	GetAbilitySystemComponent()->RegisterGameplayTagEvent(CLGameplayTags::Debuff_Fatigue, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &ACL_PlayerCharacter::OnGameplayTagNewOrRemoved);
}

//~ ACLCharacter Begin

void ACL_PlayerCharacter::Crouch(bool bClientSimulation)
{
	if (!CanCrouch())
	{
		return;
	}

	if (IsSprinting())
	{
		UnSprint();
	}
	
	Super::Crouch(bClientSimulation);
}

bool ACL_PlayerCharacter::CanCrouch() const
{
	return Super::CanCrouch() &&
		!GetCharacterTraversal()->IsDoingTraversalAction(); // Check the player isn't occupied (not traversing)
}

void ACL_PlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GetCLCharacterMovement()->OnStanceChanged.AddDynamic(this, &ACL_PlayerCharacter::OnStanceChanged);
	GetCLCharacterMovement()->OnGaitChanged.AddDynamic(this, &ACL_PlayerCharacter::OnGaitChanged);

	CharacterTraversal->OnTraversalActionStarted.AddDynamic(this, &ACL_PlayerCharacter::OnCharacterTraversalActionStarted);
	CharacterTraversal->OnTraversalActionFinished.AddDynamic(this, &ACL_PlayerCharacter::OnCharacterTraversalActionFinished);
}

void ACL_PlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateCardinalDirectionAngle();
	UpdateCardinalDirection();

	constexpr float InAirVelocityForCatchMin = -1000.f;
	constexpr float InAirVelocityForCatchMax = 750.f; // The max is much higher to catch jumping cases and not only falling cases
	if (
		GetCharacterMovement()->IsFalling() &&
		UKismetMathLibrary::InRange_FloatFloat(GetCharacterMovement()->Velocity.Z, InAirVelocityForCatchMin, InAirVelocityForCatchMax) &&
		!UKismetMathLibrary::Vector_IsNearlyZero(GetCharacterMovement()->GetCurrentAcceleration(), 0.1) // Don't traverse unless character is moving
		)
	{
		CharacterTraversal->RequestTraversalAction();
	}
	
	if (IsSprinting())
	{
		// Check if character reached maximum sprinting speed
		if (UKismetMathLibrary::VSizeXY(GetCharacterMovement()->Velocity) == GetCLCharacterMovement()->GetGaitSettings(ECL_Gait::Sprinting).MaxWalkingSpeed)
		{
			bFullySprinting = true;
		}
		
		// If character was fully sprinting and current speed got near regular running speed, then disable sprinting.
		if (
			(bFullySprinting &&
			UKismetMathLibrary::VSizeXY(GetCharacterMovement()->Velocity) <= GetCLCharacterMovement()->GetGaitSettings(ECL_Gait::Jogging).MaxWalkingSpeed) ||
			UKismetMathLibrary::NearlyEqual_FloatFloat(UKismetMathLibrary::VSizeXY(GetCharacterMovement()->Velocity), 0.f)
			)
		{
			UnSprint();
		}
	}
}

void ACL_PlayerCharacter::Die()
{
	// In the case of death we would like to first execute our logic and only then the rest of the parent's logic
	GetGameplayCamera()->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
	
	Super::Die();
}

void ACL_PlayerCharacter::OnGameplayTagNewOrRemoved(FGameplayTag GameplayTag, int NewCount)
{
	Super::OnGameplayTagNewOrRemoved(GameplayTag, NewCount);
	
	if (GameplayTag == CLGameplayTags::Debuff_Fatigue)
	{
		if (NewCount > 0)
		{
			FatigueApplied();
		}
	}
}

//~ ACLCharacter End
