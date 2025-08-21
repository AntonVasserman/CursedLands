// Copyright Anton Vasserman, All Rights Reserved.


#include "Systems/Minimap/CL_MinimapSubsystem.h"

#include "CL_LogChannels.h"
#include "Systems/Minimap/CL_MinimapSensor.h"
#include "Systems/Minimap/Components/CL_MinimapIconComponent.h"

void UCL_MinimapSubsystem::InitMinimapSensor(APawn* InNewPawn)
{
	if (IsValid(InNewPawn) == false)
	{
		return;
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = InNewPawn;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
	MinimapSensor = InNewPawn->GetWorld()->SpawnActor<ACL_MinimapSensor>(
		ACL_MinimapSensor::StaticClass(),
		InNewPawn->GetActorLocation(),
		InNewPawn->GetActorRotation(),
		SpawnParams);

	check(MinimapSensor);
	
	MinimapSensor->InitializeSensor(MinimapCollisionCapsuleRadius, MinimapCollisionCapsuleHalfHeight);
	MinimapSensor->AttachToActor(InNewPawn, FAttachmentTransformRules::KeepRelativeTransform);
		
	MinimapSensor->OnBeginOverlap.AddDynamic(this, &UCL_MinimapSubsystem::OnMinimapSensorBeginOverlap);
	MinimapSensor->OnEndOverlap.AddDynamic(this, &UCL_MinimapSubsystem::OnMinimapSensorEndOverlap);
	
	CL_LOG_MINIMAP_SYSTEM_DISPLAY("MinimapSensor created and attached to %s", *InNewPawn->GetName());
}

void UCL_MinimapSubsystem::ClearMinimapSensor(APawn* InOldPawn)
{
	// If it's the first time a Pawn was possessed, we don't need to do any cleanup
	if (IsValid(InOldPawn) == false)
	{
		return;
	}

	// If the minimap sensor is somehow already marked for cleanup, no need to do cleanup
	if (IsValid(MinimapSensor) == false)
	{
		return;
	}
	
	MinimapSensor->OnBeginOverlap.RemoveAll(this);
	MinimapSensor->OnEndOverlap.RemoveAll(this);

	MinimapSensor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		
	MinimapSensor->Destroy();
	MinimapSensor = nullptr;
		
	CL_LOG_MINIMAP_SYSTEM_DISPLAY("MinimapSensor destroyed");
}

void UCL_MinimapSubsystem::SampleActorsWithMinimapLocations()
{
	for (TPair<UCL_MinimapIconComponent*, FVector>& MinimapIconLastRelativeLocation : MinimapIconToLastRelativeLocation)
	{
		AActor* Actor = MinimapIconLastRelativeLocation.Key->GetOwner();
		FVector NewRelativeLocation = Actor->GetActorLocation() - MinimapSensor->GetActorLocation();
		if (FVector LocationDiff = NewRelativeLocation - MinimapIconLastRelativeLocation.Value;
			LocationDiff.Size() > ActorsLocationAcceptableDelta)
		{
			MinimapIconLastRelativeLocation.Value = NewRelativeLocation;
			OnActorWithMinimapIconRelativeLocationUpdated.Broadcast(MinimapIconLastRelativeLocation.Key, MinimapIconLastRelativeLocation.Value);
		}
	}
}

void UCL_MinimapSubsystem::OnPlayerControllerPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	CL_LOG_MINIMAP_SYSTEM_DISPLAY(
		"Possessed Pawn Changed, OldPawn: %s, NewPawn: %s",
		*(OldPawn != nullptr ? OldPawn->GetName() : FString("None")),
		*(NewPawn != nullptr ? NewPawn->GetName() : FString("None")));

	ClearMinimapSensor(OldPawn);
	InitMinimapSensor(NewPawn);
	CurrentPawn = NewPawn;
	OnMinimapSensorOwningPawnChanged.Broadcast(OldPawn, CurrentPawn);
}

void UCL_MinimapSubsystem::OnMinimapSensorBeginOverlap(AActor* Actor, UCL_MinimapIconComponent* MinimapIconComponent)
{
	if (IsValid(Actor))
	{
		CL_LOG_MINIMAP_SYSTEM_DISPLAY("Subsystem received BEGIN OVERLAP with %s", *Actor->GetName());
		const FVector MinimapIconRelativeLocation = Actor->GetActorLocation() - MinimapSensor->GetActorLocation();
		MinimapIconToLastRelativeLocation.Add(MinimapIconComponent, MinimapIconRelativeLocation);
		OnActorWithMinimapIconDetected.Broadcast(MinimapIconComponent, MinimapIconRelativeLocation);
		CL_LOG_MINIMAP_SYSTEM_DISPLAY("New count of Minimap Icons: %d", MinimapIconToLastRelativeLocation.Num());
	}
}

void UCL_MinimapSubsystem::OnMinimapSensorEndOverlap(AActor* Actor, UCL_MinimapIconComponent* MinimapIconComponent)
{
	if (IsValid(Actor))
	{
		CL_LOG_MINIMAP_SYSTEM_DISPLAY("Subsystem received END OVERLAP with %s", *Actor->GetName());
		MinimapIconToLastRelativeLocation.Remove(MinimapIconComponent);
		OnActorWithMinimapIconLost.Broadcast(MinimapIconComponent);
		CL_LOG_MINIMAP_SYSTEM_DISPLAY("New count of Minimap Icons: %d", MinimapIconToLastRelativeLocation.Num());
	}
}

//~ Begin ULocalPlayerSubsystem

void UCL_MinimapSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	GetWorld()->GetTimerManager().SetTimer(
		ActorsWithMinimapLocationSamplingTimer,
		this,
		&UCL_MinimapSubsystem::SampleActorsWithMinimapLocations,
		ActorsWithMinimapLocationSamplingTimerInterval,
		true);
}

void UCL_MinimapSubsystem::PlayerControllerChanged(APlayerController* NewPlayerController)
{
	Super::PlayerControllerChanged(NewPlayerController);

	CL_LOG_MINIMAP_SYSTEM_DISPLAY(
		"Changing Controller, PreviousController: %s, NewController: %s",
		*(CurrentPlayerController != nullptr ? CurrentPlayerController->GetName() : FString("None")),
		*NewPlayerController->GetName());

	if (CurrentPlayerController != nullptr)
	{
		CurrentPlayerController->OnPossessedPawnChanged.RemoveAll(this);
	}

	CurrentPlayerController = NewPlayerController;
	CurrentPlayerController->OnPossessedPawnChanged.AddDynamic(this, &UCL_MinimapSubsystem::OnPlayerControllerPossessedPawnChanged);
}

//~ End ULocalPlayerSubsystem
