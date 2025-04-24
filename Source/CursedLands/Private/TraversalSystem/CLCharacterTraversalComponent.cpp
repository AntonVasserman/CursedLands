// Copyright Anton Vasserman, All Rights Reserved.


#include "TraversalSystem/CLCharacterTraversalComponent.h"

#include "Chooser.h"
#include "ChooserFunctionLibrary.h"
#include "MotionWarpingComponent.h"
#include "Characters/CLCharacter.h"
#include "Characters/CLPlayerCharacter.h"
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
	// TODO: Remove
	CVarShowDebugCLCharacterTraversal->Set(true);
	// bool bDebug = CVarShowDebugCLCharacterTraversal->GetBool();
	// bool bDebug = 
	// 	CVarShowDebugCLCharacterTraversal->GetBool() &&
	// 	GetWorld() && GetWorld()->IsPlayInEditor();
	bool bDebug = true;
	//
	
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
	if (TraversableActor == nullptr)
	{
		return false;
	}
	
	TraversalCheckResult.HitComponent = HitResult.GetComponent();
	FLedgeCheckResult FrontLedgeCheckResult;
	FLedgeCheckResult BackLedgeCheckResult;
	TraversableActor->CheckLedges(ActorLocation, HitResult.ImpactPoint, FrontLedgeCheckResult, BackLedgeCheckResult);

	if (bDebug)
	{
		DrawDebugSphere(GetWorld(), FrontLedgeCheckResult.LedgeLocation, 10.f, 12, FColor::Green, false, 5.f, 0.f, 1.f);
		DrawDebugSphere(GetWorld(), BackLedgeCheckResult.LedgeLocation, 10.f, 12, FColor::Blue, false, 5.f, 0.f, 1.f);
	}

	// Check that the Traversable Object has a valid front ledge
	if (!FrontLedgeCheckResult.bHasLedge)
	{
		return false;
	}

	TraversalCheckResult.FrontLedgeCheckResult = FrontLedgeCheckResult;
	TraversalCheckResult.BackLedgeCheckResult = BackLedgeCheckResult;

	// Check that there is room for the Character to move up to the front ledge
	FVector FrontLedgeRoomCheckLocation;
	FHitResult FrontLedgeRoomCheckHitResult;
	bool bFrontLedgeRoomCheckHit = CapsuleTraceToCheckRoomOnLedge(ActorLocation, CapsuleRadius, CapsuleHalfHeight, TraversalCheckResult.FrontLedgeCheckResult.LedgeLocation, TraversalCheckResult.FrontLedgeCheckResult.LedgeNormal, FrontLedgeRoomCheckLocation, FrontLedgeRoomCheckHitResult, bDebug);
	if (FrontLedgeRoomCheckHitResult.bBlockingHit || FrontLedgeRoomCheckHitResult.bStartPenetrating)
	{
		// Clear the front ledge, as it is blocked
		TraversalCheckResult.FrontLedgeCheckResult = FLedgeCheckResult();
		return false;
	}

	// Evaluate the obstacle height
	FVector ActorFootLocation = ActorLocation - FVector(0.f, 0.f, CapsuleHalfHeight); 
	FVector ActorObstacleDelta = ActorFootLocation - TraversalCheckResult.FrontLedgeCheckResult.LedgeLocation;
	TraversalCheckResult.ObstacleHeight = FMath::Abs(ActorObstacleDelta.Z);

	// Check that there is room for the Character from Front ledge to Back ledge
	FVector BackLedgeRoomCheckLocation;
	FHitResult BackLedgeRoomCheckHitResult;
	bool bBackLedgeRoomCheckHit = CapsuleTraceToCheckRoomOnLedge(FrontLedgeRoomCheckLocation, CapsuleRadius, CapsuleHalfHeight, TraversalCheckResult.BackLedgeCheckResult.LedgeLocation, TraversalCheckResult.BackLedgeCheckResult.LedgeNormal, BackLedgeRoomCheckLocation, BackLedgeRoomCheckHitResult, bDebug);
	if (!bBackLedgeRoomCheckHit) // There is room, since there was no hit
	{
		// Obstacle depth is the difference between the front and back ledge locations
		FVector LedgesDelta = TraversalCheckResult.FrontLedgeCheckResult.LedgeLocation - TraversalCheckResult.BackLedgeCheckResult.LedgeLocation;
		TraversalCheckResult.ObstacleDepth = UKismetMathLibrary::VSizeXY(LedgesDelta);

		// Trace downward from the back ledge location (using the height of the obstacle for the distance) to find the floor.
		// If there is a floor, save its location and the back ledges height (using the distance between the back ledge and the floor).
		// If no floor was found, invalidate the back floor.
		FVector BackFloorCheckEndLocation = TraversalCheckResult.BackLedgeCheckResult.LedgeLocation + TraversalCheckResult.BackLedgeCheckResult.LedgeNormal * (CapsuleRadius + 2.f) - FVector(0.f, 0.f, TraversalCheckResult.ObstacleHeight);
		FHitResult BackFloorCheckHitResult;
		const EDrawDebugTrace::Type DebugType = bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
		UKismetSystemLibrary::CapsuleTraceSingle(this, BackLedgeRoomCheckLocation, BackFloorCheckEndLocation, CapsuleRadius, CapsuleHalfHeight, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, TArray<AActor*>(), DebugType, BackFloorCheckHitResult, true, FLinearColor::Yellow, FLinearColor::Yellow);
		
		if (BackFloorCheckHitResult.bBlockingHit)
		{
			TraversalCheckResult.bHasBackFloor = true;
			TraversalCheckResult.BackFloorLocation = BackFloorCheckHitResult.ImpactPoint;
			TraversalCheckResult.BackFloorHeight = FMath::Abs((BackFloorCheckHitResult.ImpactPoint - TraversalCheckResult.BackLedgeCheckResult.LedgeLocation).Z);
		}
		else
		{
			TraversalCheckResult.bHasBackFloor = false;
		}
	}
	else // There is no room, since there was a hit
	{
		// Obstacle depth is the difference between the front ledge and the trace impact point, and invalidate the back ledge.
		FVector LedgesDelta = BackLedgeRoomCheckHitResult.ImpactPoint - TraversalCheckResult.FrontLedgeCheckResult.LedgeLocation;
		TraversalCheckResult.ObstacleDepth = UKismetMathLibrary::VSizeXY(LedgesDelta);

		// Clear back ledge result as there is no room
		TraversalCheckResult.BackLedgeCheckResult = FLedgeCheckResult();
	}

	FCLTraversalChooserInput TraversalChooserInput;
	TraversalChooserInput.bHasFrontLedge = TraversalCheckResult.FrontLedgeCheckResult.bHasLedge;
	TraversalChooserInput.bHasBackLedge = TraversalCheckResult.BackLedgeCheckResult.bHasLedge;
	TraversalChooserInput.bHasBackFloor = TraversalCheckResult.bHasBackFloor;
	TraversalChooserInput.ObstacleHeight = TraversalCheckResult.ObstacleHeight;
	TraversalChooserInput.ObstacleDepth = TraversalCheckResult.ObstacleDepth;
	TraversalChooserInput.BackLedgeHeight = TraversalCheckResult.BackLedgeHeight;
	TraversalChooserInput.MovementMode = CharacterOwner->GetCharacterMovement()->MovementMode;
	TraversalChooserInput.Gait = CharacterOwner->GetCLCharacterMovement()->GetGait();
	TraversalChooserInput.Speed = UKismetMathLibrary::VSizeXY(CharacterOwner->GetCharacterMovement()->Velocity);

	FCLTraversalChooserOutput TraversalChooserOutput;
	
	FChooserEvaluationContext TraversalChooserContext;
	
	TraversalChooserContext.AddStructParam<FCLTraversalChooserInput>(TraversalChooserInput);
	TraversalChooserContext.AddStructParam<FCLTraversalChooserOutput>(TraversalChooserOutput);
	
	UObject* ChooserResult = UChooserFunctionLibrary::EvaluateObjectChooserBase(TraversalChooserContext, UChooserFunctionLibrary::MakeEvaluateChooser(TraversalAnimMontageChooserTable.Get()), UAnimMontage::StaticClass());
	UAnimMontage* TraversalAnimMontage = Cast<UAnimMontage>(ChooserResult);
	TraversalCheckResult.ChosenMontage = TraversalAnimMontage;
	TraversalCheckResult.Action = TraversalChooserOutput.ActionType;
	TraversalCheckResult.PlayRate = TraversalChooserOutput.AnimMontagePlayRate;
	
	if (TraversalCheckResult.Action == ECLTraversalAction::None)
	{
		return false;
	}

	TArray<FMotionWarpingWindowData> OutWindows;
	UMotionWarpingUtilities::GetMotionWarpingWindowsForWarpTargetFromAnimation(TraversalCheckResult.ChosenMontage, FName(TEXT("FrontLedge")), OutWindows);

	float DistanceFromFrontLedge = UKismetMathLibrary::Vector_Distance(TraversalCheckResult.FrontLedgeCheckResult.LedgeLocation, ActorLocation);
	float AnimMontageFrontLedgeMotionWarpStartTime = OutWindows[0].StartTime;
	TraversalCheckResult.StartTime = UKismetMathLibrary::MapRangeClamped(DistanceFromFrontLedge, 450.f, 0.f, 0.f, AnimMontageFrontLedgeMotionWarpStartTime);
	
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

bool UCLCharacterTraversalComponent::CapsuleTraceToCheckRoomOnLedge(const FVector& StartLocation, const float CapsuleRadius, const float CapsuleHalfHeight,
	const FVector& LedgeLocation, const FVector& LedgeNormal, FVector& OutEndLocation, FHitResult& OutHit, const bool bDebug)
{
	OutEndLocation = LedgeLocation + LedgeNormal * (CapsuleRadius + 2.f) + FVector(0.f, 0.f, CapsuleHalfHeight + 2.f);

	if (bDebug)
	{
		DrawDebugSphere(GetWorld(), OutEndLocation, 5.f, 12, FColor::Yellow, false, 5.f, 0.f, 1.f);
	}

	const FVector TraceStart = StartLocation;
	const FVector TraceEnd = OutEndLocation;
	const EDrawDebugTrace::Type DebugType = bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	return UKismetSystemLibrary::CapsuleTraceSingle(this, TraceStart, TraceEnd, CapsuleRadius, CapsuleHalfHeight, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, TArray<AActor*>(), DebugType, OutHit, true, FLinearColor::Yellow, FLinearColor::Yellow);
}

//~ UActorComponent Begin

void UCLCharacterTraversalComponent::BeginPlay()
{
	Super::BeginPlay();

	// Cache the OwnerActor, the owner of this component shouldn't be a subject of change
	CharacterOwner = Cast<ACLPlayerCharacter>(GetOwner());
	checkf(CharacterOwner, TEXT("%s failed to initialize the CharacterOwner!"), *GetName());
	
	// Check that the owner actor has the required dependency components
	OwnerMotionWarpingComponent = CharacterOwner->FindComponentByClass<UMotionWarpingComponent>();
	checkf(OwnerMotionWarpingComponent, TEXT("%s needs to have a MotionWarpingComponent for CharacterTraversalComponent function properly!"), *CharacterOwner->GetName());
}

//~ UActorComponent End
