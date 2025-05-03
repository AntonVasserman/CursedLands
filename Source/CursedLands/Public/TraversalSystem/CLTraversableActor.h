// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "CLTraversableActor.generated.h"

class USplineComponent;

USTRUCT(BlueprintType)
struct FLedgeCheckResult
{
	GENERATED_BODY()

	bool bHasLedge = false;
	FVector LedgeLocation = FVector::ZeroVector;
	FVector LedgeNormal = FVector::ZeroVector;
};

UCLASS()
class CURSEDLANDS_API ACLTraversableActor : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	ACLTraversableActor();

	UFUNCTION(BlueprintCallable, Category = "Traversal System")
	void CheckLedges(const FVector& ActorLocation, const FVector& HitLocation, FLedgeCheckResult& OutFrontLedgeCheckResult, FLedgeCheckResult& OutBackLedgeCheckResult) const;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Config|Traversal System", Meta = (AllowPrivateAccess = "true"))
	float MinLedgeWidth;
	
	UPROPERTY(BlueprintReadWrite, Category = "Traversal System", Meta = (AllowPrivateAccess = "true"))
	TArray<USplineComponent*> LedgeSplineComponents;

	UPROPERTY(BlueprintReadWrite, Category = "Traversal System", Meta = (AllowPrivateAccess = "true"))
	TMap<USplineComponent*, USplineComponent*> OppositeLedgeSplineComponents;

	USplineComponent* FindLedgeClosestToLocation(const FVector& Location) const;
	
	//~ AStaticMeshActor Begin
public:
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;	
	//~ AStaticMeshActor End
};
