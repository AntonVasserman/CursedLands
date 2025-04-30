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
#include "TraversalSystem/CLSlidingCheckResult.h"
#include "TraversalSystem/CLTraversalCheckInput.h"
#include "TraversalSystem/CLTraversalCheckResult.h"
#include "TraversalSystem/CLTraversableActor.h"

DEFINE_LOG_CATEGORY(LogCharacterTraversal);

static TAutoConsoleVariable CVarShowDebugCLCharacterTraversal(
	TEXT("CLShowDebug.CharacterTraversal"),
	false,
	TEXT("Shows the Debug information of the CLCharacterTraversalComponent class"),
	ECVF_Default);

UCLCharacterTraversalComponent::UCLCharacterTraversalComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

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
	CurrentInProgressTraversalAction = TraversalCheckResult.Action;
	CharacterOwner->GetCapsuleComponent()->IgnoreComponentWhenMoving(TraversalCheckResult.HitComponent, true);
	CharacterOwner->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
}

void UCLCharacterTraversalComponent::RequestSlidingAction()
{
	FCLSlidingCheckResult SlidingCheckResult;
	if (!ExecuteSlidingCheck(SlidingCheckResult))
	{
		return;
	}

	OwnerMotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(SlideEndLocationWarpTargetName, SlidingCheckResult.SlideEndLocation);
	checkf(SlidingAnimMontage, TEXT("SlidingAnimMontage uninitialized in object: %s"), *GetFullName());
	CharacterOwner->GetMesh()->GetAnimInstance()->Montage_Play(SlidingAnimMontage, 1.f, EMontagePlayReturnType::Duration, 0.f);

	FOnMontageBlendingOutStarted SlidingMontageBlendingOutStartedDelegate;
	SlidingMontageBlendingOutStartedDelegate.BindWeakLambda(this, [this, SlidingCheckResult](UAnimMontage* Montage, bool bInterrupted)
	{
		if (bInterrupted)
		{
			SlidingActionFinished(SlidingCheckResult);
		}
	});
	CharacterOwner->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(SlidingMontageBlendingOutStartedDelegate, SlidingAnimMontage);
	
	FOnMontageEnded SlidingMontageEndedDelegate;
	SlidingMontageEndedDelegate.BindWeakLambda(this, [this, SlidingCheckResult](UAnimMontage* Montage, bool bInterrupted)
	{
		if (!bInterrupted)
		{
			SlidingActionFinished(SlidingCheckResult);
		}
	});
	CharacterOwner->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(SlidingMontageEndedDelegate, SlidingAnimMontage);

	bDoingTraversalAction = true;
	CurrentInProgressTraversalAction = ECLTraversalAction::Slide;
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
	// TODO: Refactor this into smaller functions
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

	checkf(TraversalAnimMontageChooserTable, TEXT("TraversalAnimMontageChooserTable uninitialized in object: %s"), *GetFullName());
	UObject* ChooserResult = UChooserFunctionLibrary::EvaluateObjectChooserBase(TraversalChooserContext, UChooserFunctionLibrary::MakeEvaluateChooser(TraversalAnimMontageChooserTable.Get()), UAnimMontage::StaticClass());
	UAnimMontage* TraversalAnimMontage = Cast<UAnimMontage>(ChooserResult);
	checkf(TraversalAnimMontage, TEXT("%s failed to choose TraversalAnimMontage!"), *GetName());
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
	UE_LOG(LogCharacterTraversal, Display, TEXT("DistanceFromFrontLedge: %f"), DistanceFromFrontLedge);
	float AnimMontageFrontLedgeMotionWarpStartTime = OutWindows[0].StartTime;
	UE_LOG(LogCharacterTraversal, Display, TEXT("AnimMontageFrontLedgeMotionWarpStartTime: %f"), AnimMontageFrontLedgeMotionWarpStartTime);
	OutTraversalCheckResult.StartTime = UKismetMathLibrary::MapRangeClamped(DistanceFromFrontLedge, 450.f, 0.f, 0.f, AnimMontageFrontLedgeMotionWarpStartTime);
	UE_LOG(LogCharacterTraversal, Display, TEXT("OutTraversalCheckResult.StartTime: %f"), OutTraversalCheckResult.StartTime);
	
	return true;
}

void UCLCharacterTraversalComponent::OnMovementModeChanged(ACharacter* Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	if (Character->GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		if (bDoingTraversalAction && CurrentInProgressTraversalAction == ECLTraversalAction::Slide)
		{
			FAlphaBlendArgs AlphaBlendOutArgs;
			AlphaBlendOutArgs.BlendTime = 0.3;
			AlphaBlendOutArgs.BlendOption = EAlphaBlendOption::HermiteCubic;
			FMontageBlendSettings BlendOutSettings = FMontageBlendSettings(AlphaBlendOutArgs);
			BlendOutSettings.BlendMode = EMontageBlendMode::Inertialization;
			
			CharacterOwner->GetMesh()->GetAnimInstance()->Montage_StopWithBlendSettings(BlendOutSettings, SlidingAnimMontage);
		}
	}
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
	CurrentInProgressTraversalAction = ECLTraversalAction::None;
	CharacterOwner->GetCapsuleComponent()->IgnoreComponentWhenMoving(HitComponent, false);

	const EMovementMode MovementMode = TraversalAction == ECLTraversalAction::Vault ? MOVE_Falling : MOVE_Walking;
	CharacterOwner->GetCharacterMovement()->SetMovementMode(MovementMode);
}

bool UCLCharacterTraversalComponent::ExecuteSlidingCheck(FCLSlidingCheckResult& OutSlidingCheckResult)
{
	bool bDebug = 
		CVarShowDebugCLCharacterTraversal->GetBool() &&
		GetWorld() && GetWorld()->IsPlayInEditor();

	const FVector CharacterLocation = CharacterOwner->GetActorLocation();
	const float CharacterRadius = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const float CharacterHalfHeight = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const float CharacterCrouchCapsuleHalfHeight = CharacterOwner->GetCharacterMovement()->GetCrouchedHalfHeight();

	const FVector TraceStart = CharacterLocation - FVector(0.f, 0.f, CharacterHalfHeight - SlidingHalfHeight);
	const FVector SlidePathTraceEnd = TraceStart + CharacterOwner->GetActorForwardVector() * SlidingTraceDistance;
	const EDrawDebugTrace::Type DebugType = bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	// Check if we have enough room to Slide
	FHitResult OutHit;
	if (UKismetSystemLibrary::CapsuleTraceSingle(this, TraceStart, SlidePathTraceEnd, CharacterRadius, SlidingHalfHeight, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, TArray<AActor*>(), DebugType, OutHit, true))
	{
		return false;
	}
	
	// Check if we can slide to Fall
	const FVector FallTraceEnd = CharacterLocation + CharacterOwner->GetActorForwardVector() * SlidingTraceDistance - FVector(0.f, 0.f, 2.f * CharacterHalfHeight);
	if (!UKismetSystemLibrary::CapsuleTraceSingle(this, FallTraceEnd, FallTraceEnd, CharacterRadius, CharacterHalfHeight, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, TArray<AActor*>(), DebugType, OutHit, true))
	{
		OutSlidingCheckResult.SlideEndStance = ECLStance::Standing;
		OutSlidingCheckResult.SlideEndLocation = CharacterLocation + CharacterOwner->GetActorForwardVector() * SlidingTraceDistance - FVector(0.f, 0.f, CharacterHalfHeight);
		return true;
	}
	
	// Check if we can slide to Stand
	const FVector StandTraceEnd = CharacterLocation + CharacterOwner->GetActorForwardVector() * SlidingTraceDistance;
	if (!UKismetSystemLibrary::CapsuleTraceSingle(this, StandTraceEnd, StandTraceEnd, CharacterRadius, CharacterHalfHeight, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, TArray<AActor*>(), DebugType, OutHit, true))
	{
		OutSlidingCheckResult.SlideEndStance = ECLStance::Standing;
		OutSlidingCheckResult.SlideEndLocation = StandTraceEnd - FVector(0.f, 0.f, CharacterHalfHeight);
		return true;
	}

	// Check if we can slide to Crouch
	if (!CharacterOwner->CanCrouch())
	{
		return false;
	}
	
	const FVector CrouchTraceEnd = CharacterLocation + CharacterOwner->GetActorForwardVector() * SlidingTraceDistance - FVector(0.f, 0.f, CharacterHalfHeight - CharacterCrouchCapsuleHalfHeight);
	if (!UKismetSystemLibrary::CapsuleTraceSingle(this, CrouchTraceEnd, CrouchTraceEnd, CharacterRadius, CharacterCrouchCapsuleHalfHeight, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, TArray<AActor*>(), DebugType, OutHit, true))
	{
		OutSlidingCheckResult.SlideEndStance = ECLStance::Crouching;
		OutSlidingCheckResult.SlideEndLocation = CrouchTraceEnd - FVector(0.f, 0.f, CharacterCrouchCapsuleHalfHeight);
		return true;
	}
	
	return false;
}

void UCLCharacterTraversalComponent::SlidingActionFinished(const FCLSlidingCheckResult& SlidingCheckResult)
{
	CharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(InitialCapsuleHalfHeight);
	bDoingTraversalAction = false;
	CurrentInProgressTraversalAction = ECLTraversalAction::None;
	CurrentInProgressTraversalActionDuration = 0;

	switch (SlidingCheckResult.SlideEndStance)
	{
	case ECLStance::Standing:
		break;
	case ECLStance::Crouching:
		CharacterOwner->Crouch();
		break;
	default:
		checkNoEntry();
	}
}

//~ UActorComponent Begin

void UCLCharacterTraversalComponent::BeginPlay()
{
	Super::BeginPlay();

	// Cache the OwnerActor, the owner of this component shouldn't be a subject of change
	CharacterOwner = Cast<ACLPlayerCharacter>(GetOwner());
	checkf(CharacterOwner, TEXT("%s failed to initialize the CharacterOwner!"), *GetName());
	InitialCapsuleHalfHeight = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	CharacterOwner->MovementModeChangedDelegate.AddDynamic(this, &UCLCharacterTraversalComponent::OnMovementModeChanged);
	
	// Check that the owner actor has the required dependency components
	OwnerMotionWarpingComponent = CharacterOwner->FindComponentByClass<UMotionWarpingComponent>();
	checkf(OwnerMotionWarpingComponent, TEXT("%s needs to have a MotionWarpingComponent for CharacterTraversalComponent function properly!"), *CharacterOwner->GetName());
}

void UCLCharacterTraversalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bDoingTraversalAction && CurrentInProgressTraversalAction == ECLTraversalAction::Slide)
	{
		CurrentInProgressTraversalActionDuration += DeltaTime;
		
		float StandToSlideAlpha;
		UAnimationWarpingLibrary::GetCurveValueFromAnimation(SlidingAnimMontage, StandToSlideAlphaCurveName, CurrentInProgressTraversalActionDuration, StandToSlideAlpha);
		const float NewHalfHeight = FMath::Lerp(InitialCapsuleHalfHeight, SlidingHalfHeight, 1.f - StandToSlideAlpha);
		CharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(NewHalfHeight);
	}
}

//~ UActorComponent End
