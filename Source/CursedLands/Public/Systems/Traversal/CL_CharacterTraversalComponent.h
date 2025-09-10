// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_TraversalAction.h"
#include "Characters/Components/CL_ExtendedCharacterMovementComponent.h"
#include "Components/ActorComponent.h"
#include "CL_CharacterTraversalComponent.generated.h"

enum class ECL_TraversalAction : uint8;
enum class ECL_Gait : uint8;
struct FCL_SlidingCheckResult;
struct FCL_TraversalCheckInput;
struct FCL_TraversalCheckResult;
class ACL_TraversableActor;
class UChooserTable;
class UMotionWarpingComponent;

CURSEDLANDS_API DECLARE_LOG_CATEGORY_EXTERN(LogCharacterTraversal, Log, All); 

USTRUCT()
struct CURSEDLANDS_API FCL_TraversalChooserInput
{
	GENERATED_BODY()

	UPROPERTY()
	bool bHasFrontLedge = false;

	UPROPERTY()
	bool bHasBackLedge = false;

	UPROPERTY()
	bool bHasBackFloor = false;

	UPROPERTY()
	float ObstacleHeight = 0.f;

	UPROPERTY()
	float ObstacleDepth = 0.f;

	UPROPERTY()
	float BackLedgeHeight = 0.f;

	UPROPERTY()
	TEnumAsByte<EMovementMode> MovementMode = MOVE_None;

	UPROPERTY()
	ECL_Gait Gait = ECL_Gait::Walking;

	UPROPERTY()
	float Speed = 0.f;
};

USTRUCT()
struct CURSEDLANDS_API FCL_TraversalChooserOutput
{
	GENERATED_BODY()

	UPROPERTY()
	ECL_TraversalAction ActionType = ECL_TraversalAction::None;

	UPROPERTY()
	float AnimMontagePlayRate = 1.f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCLTraversalActionStartedDelegate, const ECL_TraversalAction, TraversalAction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCLTraversalActionFinishedDelegate, const ECL_TraversalAction, TraversalAction);

UCLASS()
class CURSEDLANDS_API UCL_CharacterTraversalComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCL_CharacterTraversalComponent();

	UPROPERTY(BlueprintAssignable, Category = "Character Traversal")
	FCLTraversalActionStartedDelegate OnTraversalActionStarted;
	UPROPERTY(BlueprintAssignable, Category = "Character Traversal")
	FCLTraversalActionFinishedDelegate OnTraversalActionFinished;
	
	UFUNCTION(BlueprintCallable, Category = "Character Traversal")
	FORCEINLINE bool CanDoTraversalAction() const { return !IsDoingTraversalAction(); }
	UFUNCTION(BlueprintCallable, Category = "Character Traversal")
	FORCEINLINE bool IsDoingTraversalAction() const { return bDoingTraversalAction; }
	UFUNCTION(BlueprintCallable, Category = "Character Traversal")
	bool RequestTraversalAction();
	UFUNCTION(BlueprintCallable, Category = "Character Traversal")
	void RequestSlidingAction();

private:
	const FName FrontLedgeWarpTargetName = TEXT("FrontLedge");
	const FName BackLedgeWarpTargetName = TEXT("BackLedge");
	const FName BackFloorWarpTargetName = TEXT("BackFloor");
	const FName DistanceFromLedgeCurveName = TEXT("Distance_From_Ledge");
	const FName SlideEndLocationWarpTargetName = TEXT("SlideEndLocation");
	const float SlidingHalfHeight = 40.f;
	const float SlidingTraceDistance = 500.f;
	
	bool bDoingTraversalAction = false;
	ECL_TraversalAction CurrentInProgressTraversalAction = ECL_TraversalAction::None;

	UPROPERTY(Transient, DuplicateTransient)
	TObjectPtr<ACL_PlayerCharacter> CharacterOwner;
	
	UPROPERTY()
	TObjectPtr<UMotionWarpingComponent> OwnerMotionWarpingComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Config|Traversal System", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UChooserTable> TraversalAnimMontageChooserTable;

	UPROPERTY(EditDefaultsOnly, Category = "Config|Traversal System", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> SlidingAnimMontage;

	FCL_TraversalCheckInput CreateTraversalCheckInput() const;
	ACL_TraversableActor* TraceForTraversableActor(FHitResult& OutHit, const bool bDebug);
	bool CapsuleTraceToCheckRoomOnLedge(const FVector& StartLocation, const float CapsuleRadius, const float CapsuleHalfHeight,
		const FVector& LedgeLocation, const FVector& LedgeNormal, FVector& OutEndLocation, FHitResult& OutHit, const bool bDebug = false);
	bool ExecuteTraversalCheck(FCL_TraversalCheckResult& OutTraversalCheckResult);
	UFUNCTION()
	void OnMovementModeChanged(ACharacter* Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode);
	void UpdateTraversalAnimMontageWarpTargets(const FCL_TraversalCheckResult& TraversalCheckResult);
	void UpdateTraversalAnimMontageFrontLedgeWarpTarget(const FCL_TraversalCheckResult& TraversalCheckResult);
	void UpdateTraversalAnimMontageBackLedgeWarpTarget(const FCL_TraversalCheckResult& TraversalCheckResult);
	void UpdateTraversalAnimMontageBackFloorWarpTarget(const FCL_TraversalCheckResult& TraversalCheckResult);
	void TraversalActionFinished(const ECL_TraversalAction TraversalAction, UPrimitiveComponent* HitComponent);
	bool ExecuteSlidingCheck(FCL_SlidingCheckResult& OutSlidingCheckResult);
	void SlidingActionFinished(const FCL_SlidingCheckResult& SlidingCheckResult);
	
	//~ UActorComponent Begin
public:
	UFUNCTION()
	virtual void BeginPlay() override;
	//~ UActorComponent End
};
