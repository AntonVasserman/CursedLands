// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Systems/Falling/Conditions/CL_FallingCondition.h"
#include "CL_CharacterFallingComponent.generated.h"

CURSEDLANDS_API DECLARE_LOG_CATEGORY_EXTERN(LogCharacterFalling, Log, All)

class UCL_LandedTaskBase;

USTRUCT()
struct CURSEDLANDS_API FCL_FallingConditionAndTasks
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	FCL_FallingCondition Condition;

	UPROPERTY(EditDefaultsOnly, Category = "Config", Meta = (BaseClass = UCL_LandedTaskBase), Instanced)
	TArray<TObjectPtr<UCL_LandedTaskBase>> LandedTasks;
};

UCLASS()
class CURSEDLANDS_API UCL_CharacterFallingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCL_CharacterFallingComponent();

	UFUNCTION(BlueprintCallable, Category = "Character Falling|Height")
	FORCEINLINE float GetFallHeight() const { return FallHeight; }
	
private:
	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TArray<FCL_FallingConditionAndTasks> ConditionsToTasks;
	
	float FallHeight = 0.f;
	float FallBeginZ = 0.f;

	UFUNCTION()
	void OnMovementModeChanged(ACharacter* Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode);

	UFUNCTION()
	void Landed(const FHitResult& Hit);
	
	//~ Begin UActorComponent
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;	
	//~ End UActorComponent
};
