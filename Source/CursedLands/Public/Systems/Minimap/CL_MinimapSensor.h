// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CL_MinimapSensor.generated.h"

class UCapsuleComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMinimapSensorOverlap, AActor*, OtherActor);

UCLASS()
class CURSEDLANDS_API ACL_MinimapSensor : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UCapsuleComponent> MinimapCollisionCapsule = nullptr;

public:
	ACL_MinimapSensor();

	FOnMinimapSensorOverlap OnBeginOverlap;
	FOnMinimapSensorOverlap OnEndOverlap;

	void InitializeSensor(float CapsuleRadius, float CapsuleHalfHeight) const;
	UFUNCTION()
	void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	//~ Begin AActor
protected:
	virtual void BeginPlay() override;
	//~ End AActor
};
