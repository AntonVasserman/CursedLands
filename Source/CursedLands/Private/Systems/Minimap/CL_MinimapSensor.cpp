// Copyright Anton Vasserman, All Rights Reserved.


#include "Systems/Minimap/CL_MinimapSensor.h"

#include "CL_LogChannels.h"
#include "Components/CapsuleComponent.h"
#include "Systems/Minimap/Components/CL_MinimapIconComponent.h"

ACL_MinimapSensor::ACL_MinimapSensor()
{
	PrimaryActorTick.bCanEverTick = false;

	MinimapCollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MinimapCollisionCapsule"));
	RootComponent = MinimapCollisionCapsule;

	MinimapCollisionCapsule->SetCapsuleRadius(1000.0f);
	MinimapCollisionCapsule->SetCapsuleHalfHeight(5000.0f);

	MinimapCollisionCapsule->SetGenerateOverlapEvents(true);
	MinimapCollisionCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MinimapCollisionCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	MinimapCollisionCapsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	MinimapCollisionCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	MinimapCollisionCapsule->SetVisibility(false);
	MinimapCollisionCapsule->SetHiddenInGame(true);
}

void ACL_MinimapSensor::InitializeSensor(float CapsuleRadius, float CapsuleHalfHeight) const
{
	if (MinimapCollisionCapsule)
	{
		MinimapCollisionCapsule->SetCapsuleRadius(CapsuleRadius);
		MinimapCollisionCapsule->SetCapsuleHalfHeight(CapsuleHalfHeight);
		CL_LOG_MINIMAP_SYSTEM_DISPLAY("MinimapSensor initialized - Radius: %f, HalfHeight: %f", CapsuleRadius, CapsuleHalfHeight);
	}
}

void ACL_MinimapSensor::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor))
	{
		CL_LOG_MINIMAP_SYSTEM_WARNING("OtherActor is Invalid");
		return;
	}

	if (UCL_MinimapIconComponent* MinimapIconComponent = OtherActor->GetComponentByClass<UCL_MinimapIconComponent>();
		IsValid(MinimapIconComponent))
	{
		CL_LOG_MINIMAP_SYSTEM_DISPLAY("Began overlap with %s", *OtherActor->GetName());
		OnBeginOverlap.Broadcast(OtherActor, MinimapIconComponent);
	}
}

void ACL_MinimapSensor::OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor))
	{
		CL_LOG_MINIMAP_SYSTEM_WARNING("OtherActor is Invalid");
		return;
	}

	if (UCL_MinimapIconComponent* MinimapIconComponent = OtherActor->GetComponentByClass<UCL_MinimapIconComponent>();
		IsValid(MinimapIconComponent))
	{
		CL_LOG_MINIMAP_SYSTEM_DISPLAY("Ended overlap with %s", *OtherActor->GetName());
		OnEndOverlap.Broadcast(OtherActor, MinimapIconComponent);
	}
}

//~ Begin AActor

void ACL_MinimapSensor::BeginPlay()
{
	Super::BeginPlay();

	MinimapCollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &ACL_MinimapSensor::OnCapsuleBeginOverlap);
	MinimapCollisionCapsule->OnComponentEndOverlap.AddDynamic(this, &ACL_MinimapSensor::OnCapsuleEndOverlap);
	
	TArray<AActor*> OverlappingActors;
	MinimapCollisionCapsule->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (UCL_MinimapIconComponent* MinimapIconComponent = Actor->GetComponentByClass<UCL_MinimapIconComponent>();
			IsValid(MinimapIconComponent))
		{
			CL_LOG_MINIMAP_SYSTEM_DISPLAY("Began overlap with %s", *Actor->GetName());
			OnBeginOverlap.Broadcast(Actor, MinimapIconComponent);
		}
	}
}

//~ End AActor
