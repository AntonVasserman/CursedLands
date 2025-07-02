// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "CL_TraversableActor.generated.h"

class USplineComponent;

USTRUCT(BlueprintType)
struct FCL_LedgeCheckResult
{
	GENERATED_BODY()

	bool bHasLedge = false;
	FVector LedgeLocation = FVector::ZeroVector;
	FVector LedgeNormal = FVector::ZeroVector;
};

UCLASS()
class CURSEDLANDS_API ACL_TraversableActor : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	ACL_TraversableActor();

	UFUNCTION(BlueprintCallable, Category = "Traversal System")
	void CheckLedges(const FVector& ActorLocation, const FVector& HitLocation, FCL_LedgeCheckResult& OutFrontLedgeCheckResult, FCL_LedgeCheckResult& OutBackLedgeCheckResult) const;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Config|Traversal System", Meta = (AllowPrivateAccess = "true"))
	float MinLedgeWidth;
	
	UPROPERTY(BlueprintReadWrite, Category = "Traversal System", Meta = (AllowPrivateAccess = "true"))
	TArray<USplineComponent*> LedgeSplineComponents;

	UPROPERTY(BlueprintReadWrite, Category = "Traversal System", Meta = (AllowPrivateAccess = "true"))
	TMap<USplineComponent*, USplineComponent*> OppositeLedgeSplineComponents;

	USplineComponent* FindLedgeClosestToLocation(const FVector& Location) const;
	
	//~ AStaticMeshActor Begin
	//~ AStaticMeshActor End
};
