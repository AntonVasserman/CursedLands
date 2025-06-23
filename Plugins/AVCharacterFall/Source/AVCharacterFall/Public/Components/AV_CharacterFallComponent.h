// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AV_FallRange.h"
#include "Components/ActorComponent.h"
#include "AV_CharacterFallComponent.generated.h"

class UAV_FallingTaskBase;
class UAV_LandedTaskBase;

AVCHARACTERFALL_API DECLARE_LOG_CATEGORY_EXTERN(LogAV_CharacterFall, Log, All);

// TODO: I don't like this name...
USTRUCT()
struct AVCHARACTERFALL_API FAV_FallingConditionAndTasks
{
	GENERATED_BODY()

	EAV_FallRangeState State = EAV_FallRangeState::Outside;
	
	UPROPERTY(EditDefaultsOnly, Category = "")
	FAV_FallRange FallRange;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (BaseClass = UAV_LandedTaskBase), Instanced)
	TArray<TObjectPtr<UAV_LandedTaskBase>> LandedTasks;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (BaseClass = UAV_FallingTaskBase), Instanced)
	TArray<TObjectPtr<UAV_FallingTaskBase>> FallingEnterTasks;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (BaseClass = UAV_FallingTaskBase), Instanced)
	TArray<TObjectPtr<UAV_FallingTaskBase>> FallingExitTasks;
};

UCLASS(MinimalAPI, Meta=(BlueprintSpawnableComponent))
class UAV_CharacterFallComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAV_CharacterFallComponent();

	UFUNCTION(BlueprintCallable, Category = "Character Fall")
	FORCEINLINE float GetFallHeight() const { return FallHeight; }
	
private:
	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TArray<FAV_FallingConditionAndTasks> ConditionsToTasks;
	
	float FallHeight = 0.f;
	float FallBeginZ = 0.f;
	
	UFUNCTION()
	void OnMovementModeChanged(ACharacter* Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode);

	UFUNCTION()
	void Landed(const FHitResult& Hit);
	
	//~ UActorComponent Begin
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void BeginPlay() override;
	//~ UActorComponent End
};
