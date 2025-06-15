// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CLCharacterFallingComponent.generated.h"

CURSEDLANDS_API DECLARE_LOG_CATEGORY_EXTERN(LogCharacterFalling, Log, All)

class UCLLandedConditionBase;
class UCLLandedTaskBase;;

USTRUCT()
struct CURSEDLANDS_API FCLLandedConditionTasksPair
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Config", Meta = (BaseClass = UCLLandedConditionBase), Instanced)
	TObjectPtr<UCLLandedConditionBase> Condition;

	UPROPERTY(EditDefaultsOnly, Category = "Config", Meta = (BaseClass = UCLLandedTaskBase), Instanced)
	TArray<TObjectPtr<UCLLandedTaskBase>> Tasks;
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
	TArray<FCLLandedConditionTasksPair> LandedTasks;
	
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
