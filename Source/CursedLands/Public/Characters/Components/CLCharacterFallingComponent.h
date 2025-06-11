// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CLCharacterFallingComponent.generated.h"

CURSEDLANDS_API DECLARE_LOG_CATEGORY_EXTERN(LogCharacterFalling, Log, All);

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
	
	float FallHeight { 0.f };
	float FallBeginZ { 0.f };

	UFUNCTION()
	void OnMovementModeChanged(ACharacter* Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode);
	
	//~ Begin UActorComponent
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;	
	//~ End UActorComponent
};
