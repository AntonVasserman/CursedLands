// Copyright Anton Vasserman, All Rights Reserved.


#include "TraversalSystem/CLTraversableActor.h"

#include "Components/SplineComponent.h"
#include "Kismet/KismetMathLibrary.h"


ACLTraversableActor::ACLTraversableActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

USplineComponent* ACLTraversableActor::FindLedgeClosestToLocation(const FVector& Location) const
{
	if (LedgeSplineComponents.IsEmpty())
	{
		return nullptr;
	}
	
	int MinClosestIndex = 0;
	FVector ClosestLocation = LedgeSplineComponents[0]->FindLocationClosestToWorldLocation(Location, ESplineCoordinateSpace::World) +
		LedgeSplineComponents[0]->FindUpVectorClosestToWorldLocation(Location, ESplineCoordinateSpace::World) * 10.f;
	float MinClosestDistance = UKismetMathLibrary::Vector_Distance(ClosestLocation, Location);

	for (int i = 1; i < LedgeSplineComponents.Num(); i++)
	{
		ClosestLocation = LedgeSplineComponents[i]->FindLocationClosestToWorldLocation(Location, ESplineCoordinateSpace::World) +
			LedgeSplineComponents[i]->FindUpVectorClosestToWorldLocation(Location, ESplineCoordinateSpace::World) * 10.f;
		if (const float ClosestDistance = UKismetMathLibrary::Vector_Distance(ClosestLocation, Location);
			ClosestDistance < MinClosestDistance)
		{
			MinClosestDistance = ClosestDistance;
			MinClosestIndex = i;
		}
	}

	return LedgeSplineComponents[MinClosestIndex];
}

void ACLTraversableActor::CheckLedges(const FVector& ActorLocation, const FVector& HitLocation, FLedgeCheckResult& OutFrontLedgeCheckResult,
	FLedgeCheckResult& OutBackLedgeCheckResult) const
{
	// Clear the Out Input Parameters
	OutFrontLedgeCheckResult = FLedgeCheckResult();
	OutBackLedgeCheckResult = FLedgeCheckResult();

	// Evaluate Front Ledge
	const USplineComponent* ClosestLedge = FindLedgeClosestToLocation(ActorLocation);
	if (ClosestLedge == nullptr || ClosestLedge->GetSplineLength() < MinLedgeWidth)
	{
		return;
	}
	
	const FVector ClosestToHitLocation = ClosestLedge->FindLocationClosestToWorldLocation(HitLocation, ESplineCoordinateSpace::Local);
	const float DistanceAlongSplineAtHitLocation = ClosestLedge->GetDistanceAlongSplineAtLocation(ClosestToHitLocation, ESplineCoordinateSpace::Local);
	const float ClampedDistanceAlongSplineAtHitLocation = FMath::Clamp(DistanceAlongSplineAtHitLocation, MinLedgeWidth / 2.f, ClosestLedge->GetSplineLength() - MinLedgeWidth / 2.f);
	const FTransform TransformAtHitDistance = ClosestLedge->GetTransformAtDistanceAlongSpline(ClampedDistanceAlongSplineAtHitLocation, ESplineCoordinateSpace::World, false);

	OutFrontLedgeCheckResult.bHasLedge = true;
	OutFrontLedgeCheckResult.LedgeLocation = TransformAtHitDistance.GetLocation();
	OutFrontLedgeCheckResult.LedgeNormal = TransformAtHitDistance.GetRotation().GetUpVector();
	
	// Evaluate Back Ledge
	if (OppositeLedgeSplineComponents.Find(ClosestLedge) == nullptr)
	{
		return;
	}

	const USplineComponent* BackLedge = OppositeLedgeSplineComponents[ClosestLedge];
	const FTransform ClosestTransformToFrontLedge = BackLedge->FindTransformClosestToWorldLocation(OutFrontLedgeCheckResult.LedgeLocation, ESplineCoordinateSpace::World, false);
	OutBackLedgeCheckResult.bHasLedge = true;
	OutBackLedgeCheckResult.LedgeLocation = ClosestTransformToFrontLedge.GetLocation();
	OutBackLedgeCheckResult.LedgeNormal = ClosestTransformToFrontLedge.GetRotation().GetUpVector();
}


//~ AStaticMeshActor Begin

void ACLTraversableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACLTraversableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//~ AStaticMeshActor End
