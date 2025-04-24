// Copyright Anton Vasserman, All Rights Reserved.


#include "TraversalSystem/CLCharacterTraversalComponent.h"

#include "AnimationWarpingLibrary.h"
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

void UCLCharacterTraversalComponent::RequestTraversalAction()
{
	FCLTraversalCheckResult TraversalCheckResult;
	if (ExecuteTraversalCheck(TraversalCheckResult) == false)
	{
		return;
	}

	UpdateTraversalAnimMontageWarpTargets(TraversalCheckResult);

	CharacterOwner->GetMesh()->GetAnimInstance()->Montage_Play(TraversalCheckResult.ChosenMontage, TraversalCheckResult.PlayRate, EMontagePlayReturnType::Duration, TraversalCheckResult.StartTime);

	FOnMontageBlendingOutStarted TraversalMontageBlendingOutStartedDelegate;
	TraversalMontageBlendingOutStartedDelegate.BindWeakLambda(this, [this, TraversalCheckResult](UAnimMontage* Montage, bool bInterrupted)
	{
		if (bInterrupted)
		{
			TraversalActionFinished(TraversalCheckResult.Action, TraversalCheckResult.HitComponent);
		}
	});
	CharacterOwner->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(TraversalMontageBlendingOutStartedDelegate, TraversalCheckResult.ChosenMontage);
	
	FOnMontageEnded TraversalMontageEndedDelegate;
	TraversalMontageEndedDelegate.BindWeakLambda(this, [this, TraversalCheckResult](UAnimMontage* Montage, bool bInterrupted)
	{
		if (!bInterrupted)
		{
			TraversalActionFinished(TraversalCheckResult.Action, TraversalCheckResult.HitComponent);
		}
	});
	CharacterOwner->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(TraversalMontageEndedDelegate, TraversalCheckResult.ChosenMontage);

	bDoingTraversalAction = true;
	CharacterOwner->GetCapsuleComponent()->IgnoreComponentWhenMoving(TraversalCheckResult.HitComponent, true);
	CharacterOwner->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
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

	return ReturnValue;
}

bool UCLCharacterTraversalComponent::TraceForTraversableObject(const FVector& ActorLocation, FHitResult& OutHit, const bool bDebug)
{
	const FCLTraversalCheckInput TraversalCheckInput = CreateTraversalCheckInput();
	const FVector TraceStart = ActorLocation + TraversalCheckInput.TraceStartOffset;
	const FVector TraceEnd = TraceStart + TraversalCheckInput.TraceForwardDirection * TraversalCheckInput.TraceForwardDistance + TraversalCheckInput.TraceEndOffset;
	const EDrawDebugTrace::Type DebugType = bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
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

bool UCLCharacterTraversalComponent::ExecuteTraversalCheck(FCLTraversalCheckResult& OutTraversalCheckResult)
{
	bool bDebug = 
		CVarShowDebugCLCharacterTraversal->GetBool() &&
		GetWorld() && GetWorld()->IsPlayInEditor();
	
	// Get basic values for use throughout the function
	const FVector ActorLocation = CharacterOwner->GetActorLocation();
	const float CapsuleRadius = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const float CapsuleHalfHeight = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	// Evaluate the Front and Back Ledges
	FHitResult HitResult;
	TraceForTraversableObject(ActorLocation, HitResult, bDebug);
	if (HitResult.bBlockingHit == false)
	{
		return false;
	}

	ACLTraversableActor* TraversableActor = Cast<ACLTraversableActor>(HitResult.GetActor());
	if (TraversableActor == nullptr)
	{
		return false;
	}
	
	OutTraversalCheckResult.HitComponent = HitResult.GetComponent();
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

	OutTraversalCheckResult.FrontLedgeCheckResult = FrontLedgeCheckResult;
	OutTraversalCheckResult.BackLedgeCheckResult = BackLedgeCheckResult;

	// Check that there is room for the Character to move up to the front ledge
	FVector FrontLedgeRoomCheckLocation;
	FHitResult FrontLedgeRoomCheckHitResult;
	bool bFrontLedgeRoomCheckHit = CapsuleTraceToCheckRoomOnLedge(ActorLocation, CapsuleRadius, CapsuleHalfHeight, OutTraversalCheckResult.FrontLedgeCheckResult.LedgeLocation, OutTraversalCheckResult.FrontLedgeCheckResult.LedgeNormal, FrontLedgeRoomCheckLocation, FrontLedgeRoomCheckHitResult, bDebug);
	if (FrontLedgeRoomCheckHitResult.bBlockingHit || FrontLedgeRoomCheckHitResult.bStartPenetrating)
	{
		// Clear the front ledge, as it is blocked
		OutTraversalCheckResult.FrontLedgeCheckResult = FLedgeCheckResult();
		return false;
	}

	// Evaluate the obstacle height
	FVector ActorFootLocation = ActorLocation - FVector(0.f, 0.f, CapsuleHalfHeight); 
	FVector ActorObstacleDelta = ActorFootLocation - OutTraversalCheckResult.FrontLedgeCheckResult.LedgeLocation;
	OutTraversalCheckResult.ObstacleHeight = FMath::Abs(ActorObstacleDelta.Z);

	// Check that there is room for the Character from Front ledge to Back ledge
	FVector BackLedgeRoomCheckLocation;
	FHitResult BackLedgeRoomCheckHitResult;
	bool bBackLedgeRoomCheckHit = CapsuleTraceToCheckRoomOnLedge(FrontLedgeRoomCheckLocation, CapsuleRadius, CapsuleHalfHeight, OutTraversalCheckResult.BackLedgeCheckResult.LedgeLocation, OutTraversalCheckResult.BackLedgeCheckResult.LedgeNormal, BackLedgeRoomCheckLocation, BackLedgeRoomCheckHitResult, bDebug);
	if (!bBackLedgeRoomCheckHit) // There is room, since there was no hit
	{
		// Obstacle depth is the difference between the front and back ledge locations
		FVector LedgesDelta = OutTraversalCheckResult.FrontLedgeCheckResult.LedgeLocation - OutTraversalCheckResult.BackLedgeCheckResult.LedgeLocation;
		OutTraversalCheckResult.ObstacleDepth = UKismetMathLibrary::VSizeXY(LedgesDelta);

		// Trace downward from the back ledge location (using the height of the obstacle for the distance) to find the floor.
		// If there is a floor, save its location and the back ledges height (using the distance between the back ledge and the floor).
		// If no floor was found, invalidate the back floor.
		FVector BackFloorCheckEndLocation = OutTraversalCheckResult.BackLedgeCheckResult.LedgeLocation + OutTraversalCheckResult.BackLedgeCheckResult.LedgeNormal * (CapsuleRadius + 2.f) - FVector(0.f, 0.f, OutTraversalCheckResult.ObstacleHeight);
		FHitResult BackFloorCheckHitResult;
		const EDrawDebugTrace::Type DebugType = bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
		UKismetSystemLibrary::CapsuleTraceSingle(this, BackLedgeRoomCheckLocation, BackFloorCheckEndLocation, CapsuleRadius, CapsuleHalfHeight, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, TArray<AActor*>(), DebugType, BackFloorCheckHitResult, true, FLinearColor::Yellow, FLinearColor::Yellow);
		
		if (BackFloorCheckHitResult.bBlockingHit)
		{
			OutTraversalCheckResult.bHasBackFloor = true;
			OutTraversalCheckResult.BackFloorLocation = BackFloorCheckHitResult.ImpactPoint;
			OutTraversalCheckResult.BackFloorHeight = FMath::Abs((BackFloorCheckHitResult.ImpactPoint - OutTraversalCheckResult.BackLedgeCheckResult.LedgeLocation).Z);
		}
		else
		{
			OutTraversalCheckResult.bHasBackFloor = false;
		}
	}
	else // There is no room, since there was a hit
	{
		// Obstacle depth is the difference between the front ledge and the trace impact point, and invalidate the back ledge.
		FVector LedgesDelta = BackLedgeRoomCheckHitResult.ImpactPoint - OutTraversalCheckResult.FrontLedgeCheckResult.LedgeLocation;
		OutTraversalCheckResult.ObstacleDepth = UKismetMathLibrary::VSizeXY(LedgesDelta);

		// Clear back ledge result as there is no room
		OutTraversalCheckResult.BackLedgeCheckResult = FLedgeCheckResult();
	}

	FCLTraversalChooserInput TraversalChooserInput;
	TraversalChooserInput.bHasFrontLedge = OutTraversalCheckResult.FrontLedgeCheckResult.bHasLedge;
	TraversalChooserInput.bHasBackLedge = OutTraversalCheckResult.BackLedgeCheckResult.bHasLedge;
	TraversalChooserInput.bHasBackFloor = OutTraversalCheckResult.bHasBackFloor;
	TraversalChooserInput.ObstacleHeight = OutTraversalCheckResult.ObstacleHeight;
	TraversalChooserInput.ObstacleDepth = OutTraversalCheckResult.ObstacleDepth;
	TraversalChooserInput.BackLedgeHeight = OutTraversalCheckResult.BackLedgeHeight;
	TraversalChooserInput.MovementMode = CharacterOwner->GetCharacterMovement()->MovementMode;
	TraversalChooserInput.Gait = CharacterOwner->GetCLCharacterMovement()->GetGait();
	TraversalChooserInput.Speed = UKismetMathLibrary::VSizeXY(CharacterOwner->GetCharacterMovement()->Velocity);

	FCLTraversalChooserOutput TraversalChooserOutput;
	
	FChooserEvaluationContext TraversalChooserContext;
	
	TraversalChooserContext.AddStructParam<FCLTraversalChooserInput>(TraversalChooserInput);
	TraversalChooserContext.AddStructParam<FCLTraversalChooserOutput>(TraversalChooserOutput);
	
	UObject* ChooserResult = UChooserFunctionLibrary::EvaluateObjectChooserBase(TraversalChooserContext, UChooserFunctionLibrary::MakeEvaluateChooser(TraversalAnimMontageChooserTable.Get()), UAnimMontage::StaticClass());
	UAnimMontage* TraversalAnimMontage = Cast<UAnimMontage>(ChooserResult);
	OutTraversalCheckResult.ChosenMontage = TraversalAnimMontage;
	OutTraversalCheckResult.Action = TraversalChooserOutput.ActionType;
	OutTraversalCheckResult.PlayRate = TraversalChooserOutput.AnimMontagePlayRate;
	
	if (OutTraversalCheckResult.Action == ECLTraversalAction::None)
	{
		return false;
	}

	TArray<FMotionWarpingWindowData> OutWindows;
	UMotionWarpingUtilities::GetMotionWarpingWindowsForWarpTargetFromAnimation(OutTraversalCheckResult.ChosenMontage, FrontLedgeWarpTargetName, OutWindows);

	float DistanceFromFrontLedge = UKismetMathLibrary::Vector_Distance(OutTraversalCheckResult.FrontLedgeCheckResult.LedgeLocation, ActorLocation);
	float AnimMontageFrontLedgeMotionWarpStartTime = OutWindows[0].StartTime;
	OutTraversalCheckResult.StartTime = UKismetMathLibrary::MapRangeClamped(DistanceFromFrontLedge, 450.f, 0.f, 0.f, AnimMontageFrontLedgeMotionWarpStartTime);
	
	return true;
}

void UCLCharacterTraversalComponent::UpdateTraversalAnimMontageWarpTargets(const FCLTraversalCheckResult& TraversalCheckResult)
{
	UpdateTraversalAnimMontageFrontLedgeWarpTarget(TraversalCheckResult);
	UpdateTraversalAnimMontageBackLedgeWarpTarget(TraversalCheckResult);
	UpdateTraversalAnimMontageBackFloorWarpTarget(TraversalCheckResult);
}

void UCLCharacterTraversalComponent::UpdateTraversalAnimMontageFrontLedgeWarpTarget(const FCLTraversalCheckResult& TraversalCheckResult)
{
	const FVector TargetLocation = TraversalCheckResult.FrontLedgeCheckResult.LedgeLocation + FVector(0.f, 0.f, 0.5f);
	const FVector NegatedNormal = TraversalCheckResult.FrontLedgeCheckResult.LedgeNormal * -1;
	const FRotator TargetRotation = FRotationMatrix::MakeFromX(NegatedNormal).Rotator();
	OwnerMotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(FrontLedgeWarpTargetName, TargetLocation, TargetRotation);
}

void UCLCharacterTraversalComponent::UpdateTraversalAnimMontageBackLedgeWarpTarget(const FCLTraversalCheckResult& TraversalCheckResult)
{
	if (TraversalCheckResult.Action != ECLTraversalAction::Hurdle && TraversalCheckResult.Action != ECLTraversalAction::Vault)
	{
		OwnerMotionWarpingComponent->RemoveWarpTarget(BackLedgeWarpTargetName);
		return;
	}

	TArray<FMotionWarpingWindowData> MotionWarpingWindows;
	UMotionWarpingUtilities::GetMotionWarpingWindowsForWarpTargetFromAnimation(TraversalCheckResult.ChosenMontage, BackLedgeWarpTargetName, MotionWarpingWindows);
	if (MotionWarpingWindows.IsEmpty())
	{
		OwnerMotionWarpingComponent->RemoveWarpTarget(BackLedgeWarpTargetName);
		return;
	}

	OwnerMotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(BackLedgeWarpTargetName, TraversalCheckResult.BackLedgeCheckResult.LedgeLocation, FRotator::ZeroRotator);
}

void UCLCharacterTraversalComponent::UpdateTraversalAnimMontageBackFloorWarpTarget(const FCLTraversalCheckResult& TraversalCheckResult)
{
	if (TraversalCheckResult.Action != ECLTraversalAction::Hurdle)
	{
		OwnerMotionWarpingComponent->RemoveWarpTarget(BackFloorWarpTargetName);
		return;
	}
	
	TArray<FMotionWarpingWindowData> BackFloorMotionWarpingWindows;
	UMotionWarpingUtilities::GetMotionWarpingWindowsForWarpTargetFromAnimation(TraversalCheckResult.ChosenMontage, BackFloorWarpTargetName, BackFloorMotionWarpingWindows);
	if (BackFloorMotionWarpingWindows.IsEmpty())
	{
		OwnerMotionWarpingComponent->RemoveWarpTarget(BackFloorWarpTargetName);
		return;
	}

	float DistanceFromFrontLedgeToBackFloor;
	UAnimationWarpingLibrary::GetCurveValueFromAnimation(TraversalCheckResult.ChosenMontage, DistanceFromLedgeCurveName, BackFloorMotionWarpingWindows[0].EndTime, DistanceFromFrontLedgeToBackFloor);

	TArray<FMotionWarpingWindowData> BackLedgeMotionWarpingWindows;
	UMotionWarpingUtilities::GetMotionWarpingWindowsForWarpTargetFromAnimation(TraversalCheckResult.ChosenMontage, BackLedgeWarpTargetName, BackLedgeMotionWarpingWindows);
	float DistanceFromFrontLedgeToBackLedge;
	UAnimationWarpingLibrary::GetCurveValueFromAnimation(TraversalCheckResult.ChosenMontage, DistanceFromLedgeCurveName, BackLedgeMotionWarpingWindows[0].EndTime, DistanceFromFrontLedgeToBackLedge);

	const FVector AdjustedBackLedgeLocation = TraversalCheckResult.BackLedgeCheckResult.LedgeLocation + TraversalCheckResult.BackLedgeCheckResult.LedgeNormal * FMath::Abs(DistanceFromFrontLedgeToBackLedge - DistanceFromFrontLedgeToBackFloor);
	const FVector TargetLocation = FVector(AdjustedBackLedgeLocation.X, AdjustedBackLedgeLocation.Y, TraversalCheckResult.BackFloorLocation.Z);
	OwnerMotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(BackFloorWarpTargetName, TargetLocation, FRotator::ZeroRotator);
}

void UCLCharacterTraversalComponent::TraversalActionFinished(const ECLTraversalAction TraversalAction, UPrimitiveComponent* HitComponent)
{
	bDoingTraversalAction = false;
	CharacterOwner->GetCapsuleComponent()->IgnoreComponentWhenMoving(HitComponent, false);

	const EMovementMode MovementMode = TraversalAction == ECLTraversalAction::Vault ? MOVE_Falling : MOVE_Walking;
	CharacterOwner->GetCharacterMovement()->SetMovementMode(MovementMode);
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
