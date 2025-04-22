// Copyright Anton Vasserman, All Rights Reserved.


#include "TraversalSystem/CLCharacterTraversalComponent.h"

#include "MotionWarpingComponent.h"
#include "Characters/CLCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Physics/CLCollisionChannels.h"
#include "TraversalSystem/CLTraversalCheckInput.h"
#include "TraversalSystem/CLTraversalCheckResult.h"
#include "TraversalSystem/CLTraversableActor.h"

static TAutoConsoleVariable CVarShowDebugCLCharacterTraversal(
	TEXT("CLShowDebug.CharacterTraversal"),
	false,
	TEXT("Shows the Debug information of the CLCharacterTraversalComponent class"),
	ECVF_Default);

UCLCharacterTraversalComponent::UCLCharacterTraversalComponent()
{
}

bool UCLCharacterTraversalComponent::TryTraversalAction()
{
	FCLTraversalCheckResult TraversalCheckResult;
	
	// Get basic values for use throughout the function
	const FVector ActorLocation = CharacterOwner->GetActorLocation();
	const float CapsuleRadius = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const float CapsuleHalfHeight = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	// Evaluate the Front and Back Ledges
	FHitResult HitResult;
	TraceForTraversableObject(ActorLocation, HitResult);
	if (HitResult.bBlockingHit == false)
	{
		return false;
	}

	ACLTraversableActor* TraversableActor = Cast<ACLTraversableActor>(HitResult.GetActor());

	if (Cast<ACLTraversableActor>(HitResult.GetActor()) == nullptr)
	{
		return false;
	}
	
	TraversalCheckResult.HitComponent = HitResult.GetComponent();
	FLedgeCheckResult FrontLedgeCheckResult;
	FLedgeCheckResult BackLedgeCheckResult;
	TraversableActor->CheckLedges(ActorLocation, HitResult.ImpactPoint, FrontLedgeCheckResult, BackLedgeCheckResult);

	if (
		CVarShowDebugCLCharacterTraversal->GetBool() &&
		GetWorld() && GetWorld()->IsPlayInEditor()
		)
	{
		DrawDebugSphere(GetWorld(), FrontLedgeCheckResult.LedgeLocation, 10.f, 12, FColor::Green, false, 5.f, 0.f, 1.f);
		DrawDebugSphere(GetWorld(), BackLedgeCheckResult.LedgeLocation, 10.f, 12, FColor::Blue, false, 5.f, 0.f, 1.f);
	}
	
	return true;
}

FCLTraversalCheckInput UCLCharacterTraversalComponent::CreateTraversalCheckInput() const
{
	FCLTraversalCheckInput ReturnValue;

	ReturnValue.TraceForwardDirection = CharacterOwner->GetActorForwardVector();
	ReturnValue.TraceStartOffset = FVector::ZeroVector;
	ReturnValue.TraceRadius = 30.f;
	
	if (CharacterOwner->GetCharacterMovement()->MovementMode == MOVE_Falling ||
		CharacterOwner->GetCharacterMovement()->MovementMode == MOVE_Flying)
	{
		ReturnValue.TraceForwardDistance = 75.f;
		ReturnValue.TraceEndOffset = FVector(0.f, 0.f, 50.f);
		ReturnValue.TraceHalfHeight = 86.f;
	}
	else
	{
		const FRotator Rotation = CharacterOwner->GetActorRotation();
		const FVector Velocity = CharacterOwner->GetCharacterMovement()->Velocity;
		const float TraceForwardDistance = Rotation.UnrotateVector(Velocity).X;
		const float TraceForwardDistanceClampedInRange = UKismetMathLibrary::MapRangeClamped(TraceForwardDistance, 0.f, 500.f, 75.f, 350.f);
		
		ReturnValue.TraceForwardDistance = TraceForwardDistanceClampedInRange;
		ReturnValue.TraceEndOffset = FVector::ZeroVector;
		ReturnValue.TraceHalfHeight = 60.f;
	}

	if (CVarShowDebugCLCharacterTraversal->GetBool() && GEngine)
	{
		const FColor TextColor = FColor::Red;
		const FVector2D TextScale = FVector2D(1.5f, 1.5f);

		GEngine->AddOnScreenDebugMessage(200, 5.0f, TextColor, FString::Printf(TEXT("CreateTraversalCheckInput::TraceForwardDirection: %s"), *ReturnValue.TraceForwardDirection.ToString()), false, TextScale);
		GEngine->AddOnScreenDebugMessage(201, 5.0f, TextColor, FString::Printf(TEXT("CreateTraversalCheckInput::TraceForwardDistance: %f"), ReturnValue.TraceForwardDistance), false, TextScale);
		GEngine->AddOnScreenDebugMessage(202, 5.0f, TextColor, FString::Printf(TEXT("CreateTraversalCheckInput::TraceStartOffset: %s"), *ReturnValue.TraceStartOffset.ToString()), false, TextScale);
		GEngine->AddOnScreenDebugMessage(203, 5.0f, TextColor, FString::Printf(TEXT("CreateTraversalCheckInput::TraceEndOffset: %s"), *ReturnValue.TraceEndOffset.ToString()), false, TextScale);
		GEngine->AddOnScreenDebugMessage(204, 5.0f, TextColor, FString::Printf(TEXT("CreateTraversalCheckInput::TraceRadius: %f"), ReturnValue.TraceRadius), false, TextScale);
		GEngine->AddOnScreenDebugMessage(205, 5.0f, TextColor, FString::Printf(TEXT("CreateTraversalCheckInput::TraceHalfHeight: %f"), ReturnValue.TraceHalfHeight), false, TextScale);
	}
	
	return ReturnValue;
}

bool UCLCharacterTraversalComponent::TraceForTraversableObject(const FVector& ActorLocation, FHitResult& OutHit)
{
	const FCLTraversalCheckInput TraversalCheckInput = CreateTraversalCheckInput();
	const FVector TraceStart = ActorLocation + TraversalCheckInput.TraceStartOffset;
	const FVector TraceEnd = TraceStart + TraversalCheckInput.TraceForwardDirection * TraversalCheckInput.TraceForwardDistance + TraversalCheckInput.TraceEndOffset;
	const EDrawDebugTrace::Type DebugType = CVarShowDebugCLCharacterTraversal->GetBool() ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	return UKismetSystemLibrary::CapsuleTraceSingle(this, TraceStart, TraceEnd, TraversalCheckInput.TraceRadius, TraversalCheckInput.TraceHalfHeight, CL_TraceTypeQuery_Traversability, false, TArray<AActor*>(), DebugType, OutHit, true);
}

//~ UActorComponent Begin

void UCLCharacterTraversalComponent::BeginPlay()
{
	Super::BeginPlay();

	// Cache the OwnerActor, the owner of this component shouldn't be a subject of change
	CharacterOwner = Cast<ACLCharacter>(GetOwner());
	checkf(CharacterOwner, TEXT("%s failed to initialize the CharacterOwner!"), *GetName());
	
	// Check that the owner actor has the required dependency components
	OwnerMotionWarpingComponent = CharacterOwner->FindComponentByClass<UMotionWarpingComponent>();
	checkf(OwnerMotionWarpingComponent, TEXT("%s needs to have a MotionWarpingComponent for CharacterTraversalComponent function properly!"), *CharacterOwner->GetName());
}

//~ UActorComponent End
