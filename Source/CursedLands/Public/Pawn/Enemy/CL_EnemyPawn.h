// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pawn/CL_Pawn.h"
#include "CL_EnemyPawn.generated.h"

struct FGameplayTag;
class UCL_AbilitySystemComponent;
class UCL_HealthComponent;
class UCL_PawnData;

UCLASS()
class CURSEDLANDS_API ACL_EnemyPawn : public ACL_Pawn
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability System")
	TObjectPtr<UCL_AbilitySystemComponent> AbilitySystem;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Gameplay Ability System|Health", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCL_HealthComponent> HealthComponent;

public:
	ACL_EnemyPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(EditAnywhere, Category = "Config|Pawn")
	TObjectPtr<const UCL_PawnData> PawnData;

	// We do not just use BlueprintNativeEvent here because we want to enforce the Die function to run first and execute
	// any crucial C++ logic before we delegate to the Blueprint logic.
	UFUNCTION()
	virtual void Die();
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Die")
	void Die_BP();

	virtual void OnGameplayTagNewOrRemoved(FGameplayTag GameplayTag, int NewCount);

	//~ ACL_Pawn Begin
public:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	//~ ACL_Pawn End
};
