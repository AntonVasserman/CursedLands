// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AV_CharacterFallComponent.generated.h"

struct FAV_FallRangeTasks;

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
	TArray<FAV_FallRangeTasks> FallRangesTasks;
	
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
