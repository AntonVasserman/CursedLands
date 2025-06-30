// Copyright Anton Vasserman, All Rights Reserved.

#include "Components/AV_CharacterFallComponent.h"

#include "AV_CharacterFallLogChannels.h"
#include "Components/Models/AV_FallRangeTasks.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

static TAutoConsoleVariable CVarShowDebugAVCharacterFallComponent(
	TEXT("AVShowDebug.CharacterFallComponent"),
	false,
	TEXT("Shows the Debug information of the CharacterFallComponent class"),
	ECVF_Default);

UAV_CharacterFallComponent::UAV_CharacterFallComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAV_CharacterFallComponent::OnMovementModeChanged(ACharacter* Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	if (const EMovementMode CurrentMovementMode = Character->GetCharacterMovement()->MovementMode;
		CurrentMovementMode == MOVE_Falling)
	{
		bEvaluateFallBeginZ = true;
		UE_LOG(LogAV_CharacterFall, Display, TEXT("%s::%hs: Evaluation of Fall Height requested"), *GetClass()->GetName(), __FUNCTION__);
		if (CVarShowDebugAVCharacterFallComponent->GetBool())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Evaluation of Fall Height requested"));
		}
	}
}

void UAV_CharacterFallComponent::Landed(const FHitResult& Hit)
{
	UE_LOG(LogAV_CharacterFall, Display, TEXT("%s::%hs: Landed with Fall Height: %f"), *GetClass()->GetName(), __FUNCTION__, FallHeight);
	if (CVarShowDebugAVCharacterFallComponent->GetBool())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Landed with Fall Height: %f"), FallHeight));
	}

	FAV_FallRangeContext FallRangeContext;
	FallRangeContext.Character = OwnerCharacter;
	FallRangeContext.CharacterFallComponent = this;
	FallRangeContext.FallHeight = FallHeight;
	FallRangeContext.HitActor = Hit.GetActor();
		
	for (FAV_FallRangeTasks& FallRangeTasks : FallRangesTasks)
	{
		FallRangeTasks.TestRange(FallRangeContext);
	}
}

//~ UActorComponent Begin

void UAV_CharacterFallComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (OwnerCharacter->GetCharacterMovement()->MovementMode == MOVE_Falling && GetOwner()->GetVelocity().Z < 0.f)
	{
		if (bEvaluateFallBeginZ)
		{
			FallBeginZ = GetOwner()->GetActorLocation().Z;
			UE_LOG(LogAV_CharacterFall, Display, TEXT("%s::%hs: Starting evaluating Fall Height, with Fall Begin Z: %f"), *GetClass()->GetName(), __FUNCTION__, FallBeginZ);
			if (CVarShowDebugAVCharacterFallComponent->GetBool())
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Starting evaluating Fall Height, with Fall Begin Z: %f"), FallBeginZ));
			}
			
			bEvaluateFallBeginZ = false;
		}
		else
		{
			FallHeight = FallBeginZ - GetOwner()->GetActorLocation().Z;

			FAV_FallRangeContext FallRangeContext;
			FallRangeContext.Character = OwnerCharacter;
			FallRangeContext.CharacterFallComponent = this;
			FallRangeContext.FallHeight = FallHeight;
		
			for (FAV_FallRangeTasks& FallRangeTasks : FallRangesTasks)
			{
				FallRangeTasks.TestRange(FallRangeContext);
			}
		}
	}
}

void UAV_CharacterFallComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = CastChecked<ACharacter>(GetOwner());
	OwnerCharacter->MovementModeChangedDelegate.AddDynamic(this, &UAV_CharacterFallComponent::OnMovementModeChanged);
	OwnerCharacter->LandedDelegate.AddDynamic(this, &UAV_CharacterFallComponent::Landed);
}

//~ UActorComponent End