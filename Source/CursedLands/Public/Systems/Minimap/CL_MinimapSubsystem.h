// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "CL_MinimapSubsystem.generated.h"

class ACL_MinimapSensor;
class UCapsuleComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMinimapSensorOwningPawnChanged, APawn*, OldPawn, APawn*, NewPawn);

UCLASS()
class CURSEDLANDS_API UCL_MinimapSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<ACL_MinimapSensor> MinimapSensor = nullptr;

public:
	UPROPERTY(BlueprintAssignable)
	FOnMinimapSensorOwningPawnChanged OnMinimapSensorOwningPawnChanged;
	
private:
	UPROPERTY()
	APlayerController* CurrentPlayerController = nullptr;

	UPROPERTY()
	APawn* CurrentPawn = nullptr;

	UPROPERTY()
	TSet<AActor*> OverlappingActors;

	float MinimapCollisionCapsuleRadius = 1000.f;
	float MinimapCollisionCapsuleHalfHeight = 5000.f;
	
	void InitMinimapSensor(APawn* InNewPawn);
	void ClearMinimapSensor(APawn* InOldPawn);
	
	UFUNCTION()
	void OnPlayerControllerPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);
	UFUNCTION()
	void OnMinimapSensorBeginOverlap(AActor* OtherActor);
	UFUNCTION()
	void OnMinimapSensorEndOverlap(AActor* OtherActor);
	
	//~ Begin ULocalPlayerSubsystem
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void PlayerControllerChanged(APlayerController* NewPlayerController) override;
	//~ End ULocalPlayerSubsystem
};
