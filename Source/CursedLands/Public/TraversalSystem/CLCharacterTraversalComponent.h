// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CLTraversalAction.h"
#include "Characters/CLCharacterMovementComponent.h"
#include "Components/ActorComponent.h"
#include "CLCharacterTraversalComponent.generated.h"

class ACLCharacter;
enum class ECLTraversalAction : uint8;
enum class ECLGait : uint8;
class UChooserTable;
class ACLTraversableActor;
struct FCLTraversalCheckResult;
struct FCLTraversalCheckInput;
class UCharacterMovementComponent;
class UMotionWarpingComponent;

USTRUCT(BlueprintType)
struct CURSEDLANDS_API FCLTraversalChooserInput
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal System", Meta = (AllowPrivateAccess = "true"))
	bool bHasFrontLedge = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal System", Meta = (AllowPrivateAccess = "true"))
	bool bHasBackLedge = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal System", Meta = (AllowPrivateAccess = "true"))
	bool bHasBackFloor = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal System", Meta = (AllowPrivateAccess = "true"))
	float ObstacleHeight = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal System", Meta = (AllowPrivateAccess = "true"))
	float ObstacleDepth = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal System", Meta = (AllowPrivateAccess = "true"))
	float BackLedgeHeight = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal System", Meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EMovementMode> MovementMode = MOVE_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal System", Meta = (AllowPrivateAccess = "true"))
	ECLGait Gait = ECLGait::Walking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal System", Meta = (AllowPrivateAccess = "true"))
	float Speed = 0.f;
};

USTRUCT(BlueprintType)
struct CURSEDLANDS_API FCLTraversalChooserOutput
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal System", Meta = (AllowPrivateAccess = "true"))
	ECLTraversalAction ActionType = ECLTraversalAction::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traversal System", Meta = (AllowPrivateAccess = "true"))
	float AnimMontagePlayRate = 1.f;
};

UCLASS()
class CURSEDLANDS_API UCLCharacterTraversalComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Character Traversal")
	FORCEINLINE bool CanDoTraversalAction() const { return !IsDoingTraversalAction(); }
	UFUNCTION(BlueprintCallable, Category = "Character Traversal")
	FORCEINLINE bool IsDoingTraversalAction() const { return bDoingTraversalAction; }
	UFUNCTION(BlueprintCallable, Category = "Character Traversal")
	void RequestTraversalAction();

private:
	const FName FrontLedgeWarpTargetName = TEXT("FrontLedge");
	const FName BackLedgeWarpTargetName = TEXT("BackLedge");
	const FName BackFloorWarpTargetName = TEXT("BackFloor");
	const FName DistanceFromLedgeCurveName = TEXT("Distance_From_Ledge");
	
	bool bDoingTraversalAction = false;

	UPROPERTY(Transient, DuplicateTransient)
	TObjectPtr<ACLPlayerCharacter> CharacterOwner;
	
	UPROPERTY()
	TObjectPtr<UMotionWarpingComponent> OwnerMotionWarpingComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Config|Traversal System", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UChooserTable> TraversalAnimMontageChooserTable;

	FCLTraversalCheckInput CreateTraversalCheckInput() const;
	bool TraceForTraversableObject(const FVector& ActorLocation, FHitResult& OutHit, const bool bDebug);
	bool CapsuleTraceToCheckRoomOnLedge(const FVector& StartLocation, const float CapsuleRadius, const float CapsuleHalfHeight,
		const FVector& LedgeLocation, const FVector& LedgeNormal, FVector& OutEndLocation, FHitResult& OutHit, const bool bDebug = false);
	bool ExecuteTraversalCheck(FCLTraversalCheckResult& OutTraversalCheckResult);
	void UpdateTraversalAnimMontageWarpTargets(const FCLTraversalCheckResult& TraversalCheckResult);
	void UpdateTraversalAnimMontageFrontLedgeWarpTarget(const FCLTraversalCheckResult& TraversalCheckResult);
	void UpdateTraversalAnimMontageBackLedgeWarpTarget(const FCLTraversalCheckResult& TraversalCheckResult);
	void UpdateTraversalAnimMontageBackFloorWarpTarget(const FCLTraversalCheckResult& TraversalCheckResult);
	void TraversalActionFinished(const ECLTraversalAction TraversalAction, UPrimitiveComponent* HitComponent);
	
	//~ UActorComponent Begin
public:
	virtual void BeginPlay() override;
	//~ UActorComponent End
};
