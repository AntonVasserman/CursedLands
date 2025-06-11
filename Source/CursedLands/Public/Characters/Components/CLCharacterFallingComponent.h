// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CLCharacterFallingComponent.generated.h"

CURSEDLANDS_API DECLARE_LOG_CATEGORY_EXTERN(LogCharacterFalling, Log, All)

class UCLPrintStringLandedTask;
class UCLCheckFallHeightLandedCondition;;

// TODO: We need something other than UPrimaryDataAsset here...
UCLASS()
class CURSEDLANDS_API UCLBlabla : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	TObjectPtr<UCLCheckFallHeightLandedCondition> Condition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	TArray<TObjectPtr<UCLPrintStringLandedTask>> Tasks;
};

UCLASS()
class CURSEDLANDS_API UCLCharacterFallingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCLCharacterFallingComponent();

	UFUNCTION(BlueprintCallable, Category = "Character Falling|Height")
	FORCEINLINE float GetFallHeight() const { return FallHeight; }
	
private:
	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter {nullptr};

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TArray<TObjectPtr<UCLBlabla>> LandedTasks;
	
	float FallHeight { 0.f };
	float FallBeginZ { 0.f };

	UFUNCTION()
	void OnMovementModeChanged(ACharacter* Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode);

	void Landed(ACharacter* Character);
	
	//~ Begin UActorComponent
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;	
	//~ End UActorComponent
};
