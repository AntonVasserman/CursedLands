// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/Components/CLCharacterFallingComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Systems/Falling/Conditions/CLFallHeightInRangeLandedCondition.h"
#include "Systems/Falling/Tasks/CLPrintStringLandedTask.h"

DEFINE_LOG_CATEGORY(LogCharacterFalling);

static TAutoConsoleVariable CVarShowDebugUCLCharacterFallingComponent(
	TEXT("CLShowDebug.CharacterFallingComponent"),
	false,
	TEXT("Shows the Debug information of the CLCharacterFallingComponent class"),
	ECVF_Default);

UCLCharacterFallingComponent::UCLCharacterFallingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCLCharacterFallingComponent::OnMovementModeChanged(ACharacter* Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	if (const EMovementMode CurrentMovementMode = Character->GetCharacterMovement()->MovementMode;
		CurrentMovementMode == MOVE_Falling)
	{
		FallBeginZ = Character->GetActorLocation().Z;
	}
}

void UCLCharacterFallingComponent::Landed(const FHitResult& Hit)
{
	if (CVarShowDebugUCLCharacterFallingComponent->GetBool())
	{
		UE_LOG(LogCharacterFalling, Display, TEXT("Falling: %f"), FallHeight);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Falling: %f"), FallHeight));
	}

	for (FCLLandedConditionTasksPair Pair : LandedTasks)
	{
		FCLLandedConditionContext ConditionContext;
		ConditionContext.Character = OwnerCharacter;
		ConditionContext.HitActor = Hit.GetActor();
		ConditionContext.FallHeight = FallHeight;
		
		if (Pair.Condition->TestCondition(ConditionContext))
		{
			for (const UCLLandedTaskBase* Task : Pair.Tasks)
			{
				FCLLandedTaskContext TaskContext;
				TaskContext.Condition = Pair.Condition;
				TaskContext.FallHeight = FallHeight;
				TaskContext.Character = OwnerCharacter;
				TaskContext.FallingComponent = this;
				
				Task->ExecuteTask(TaskContext);
			}
		}
	}
}

//~ Begin UActorComponent

void UCLCharacterFallingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// We want to always evaluate FallHeight as we allow using it for multiple functionalities 
	if (OwnerCharacter->GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		FallHeight = FallBeginZ - GetOwner()->GetActorLocation().Z;
	}
}

void UCLCharacterFallingComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = CastChecked<ACharacter>(GetOwner());
	OwnerCharacter->MovementModeChangedDelegate.AddDynamic(this, &UCLCharacterFallingComponent::OnMovementModeChanged);
	OwnerCharacter->LandedDelegate.AddDynamic(this, &UCLCharacterFallingComponent::Landed);
}

//~ End UActorComponent
