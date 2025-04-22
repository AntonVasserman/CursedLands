// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CLCharacterTraversalComponent.generated.h"

class ACLTraversableActor;
struct FCLTraversalCheckResult;
struct FCLTraversalCheckInput;
class UCharacterMovementComponent;
class UMotionWarpingComponent;

UCLASS()
class CURSEDLANDS_API UCLCharacterTraversalComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCLCharacterTraversalComponent();

	UFUNCTION(BlueprintCallable, Category = "Character Traversal")
	FORCEINLINE bool CanDoTraversalAction() const { return !bDoingTraversalAction; }
	UFUNCTION(BlueprintCallable, Category = "Character Traversal")
	bool TryTraversalAction();

	// TODO: Move to private a the end of Cpp migration
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character Traversal|TODO Temporary Private Logic")
	FCLTraversalCheckInput CreateTraversalCheckInput() const;
	UFUNCTION(BlueprintCallable, Category = "Character Traversal|TODO Temporary Private Logic")
	bool TraceForTraversableObject(const FVector& ActorLocation, FHitResult& OutHit);
	//
	
private:
	bool bDoingTraversalAction = false;

	UPROPERTY(Transient, DuplicateTransient)
	TObjectPtr<ACharacter> CharacterOwner;
	
	UPROPERTY()
	TObjectPtr<UMotionWarpingComponent> OwnerMotionWarpingComponent = nullptr;

	//~ UActorComponent Begin
public:
	virtual void BeginPlay() override;
	//~ UActorComponent End
};
