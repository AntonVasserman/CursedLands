// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Settings/CL_GameLocalUserSettings.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "CL_MinimapSubsystem.generated.h"

class UCL_MinimapIconComponent;
class ACL_MinimapSensor;
class UCapsuleComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMinimapSensorOwningPawnChanged, APawn*, OldPawn, APawn*, NewPawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorWithMinimapIconDetected, UCL_MinimapIconComponent*, MinimapIconComponent, FVector, Location);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorWithMinimapIconRelativeLocationUpdated, UCL_MinimapIconComponent*, MinimapIconComponent, FVector, NewLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorWithMinimapIconLost, UCL_MinimapIconComponent*, MinimapIconComponent);

UCLASS()
class CURSEDLANDS_API UCL_MinimapSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<ACL_MinimapSensor> MinimapSensor = nullptr;

public:
	UPROPERTY(BlueprintAssignable)
	FOnMinimapSensorOwningPawnChanged OnMinimapSensorOwningPawnChanged;
	UPROPERTY(BlueprintAssignable)
	FOnActorWithMinimapIconDetected OnActorWithMinimapIconDetected;
	UPROPERTY(BlueprintAssignable)
	FOnActorWithMinimapIconRelativeLocationUpdated OnActorWithMinimapIconRelativeLocationUpdated;
	UPROPERTY(BlueprintAssignable)
	FOnActorWithMinimapIconLost OnActorWithMinimapIconLost;

	UPROPERTY(BlueprintAssignable)
	FCL_OnRotateMinimapChanged OnRotateMinimapChanged;

	UFUNCTION(BlueprintCallable, Category = "Minimap")
	FORCEINLINE float GetMinimapCollisionCapsuleRadius() const { return MinimapCollisionCapsuleRadius; }
	UFUNCTION(BlueprintCallable, Category = "Minimap")
	FORCEINLINE float GetMinimapCollisionCapsuleHalfHeight() const { return MinimapCollisionCapsuleHalfHeight; }
	
	UFUNCTION(BlueprintCallable, Category = "Minimap|Settings")
	FORCEINLINE bool GetRotateMinimap() const;
	
private:
	UPROPERTY()
	APlayerController* CurrentPlayerController = nullptr;

	UPROPERTY()
	APawn* CurrentPawn = nullptr;

	UPROPERTY()
	TMap<UCL_MinimapIconComponent*, FVector> MinimapIconToLastRelativeLocation;

	float MinimapCollisionCapsuleRadius = 1000.f;
	float MinimapCollisionCapsuleHalfHeight = 5000.f;

	FTimerHandle ActorsWithMinimapLocationSamplingTimer;
	float ActorsWithMinimapLocationSamplingTimerInterval = 0.01f;
	float ActorsLocationAcceptableDelta = 5.f;
	
	void InitMinimapSensor(APawn* InNewPawn);
	void ClearMinimapSensor(APawn* InOldPawn);
	void SampleActorsWithMinimapLocations();
	
	UFUNCTION()
	void OnPlayerControllerPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);
	UFUNCTION()
	void OnMinimapSensorBeginOverlap(AActor* Actor, UCL_MinimapIconComponent* MinimapIconComponent);
	UFUNCTION()
	void OnMinimapSensorEndOverlap(AActor* Actor, UCL_MinimapIconComponent* MinimapIconComponent);
	
	UFUNCTION()
	void LocalSettings_OnRotateMinimapChanged(bool bNewRotateMinimap);
	
	//~ Begin ULocalPlayerSubsystem
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void PlayerControllerChanged(APlayerController* NewPlayerController) override;
	//~ End ULocalPlayerSubsystem
};
