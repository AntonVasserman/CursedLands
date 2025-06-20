// Copyright Anton Vasserman, All Rights Reserved.


#include "Systems/Falling/Components/CL_CharacterFallingComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Systems/Falling/Conditions/CL_FallingCondition.h"
#include "Systems/Falling/Tasks/CL_FallingTaskBase.h"
#include "Systems/Falling/Tasks/CL_PrintStringLandedTask.h"

DEFINE_LOG_CATEGORY(LogCharacterFalling);

static TAutoConsoleVariable CVarShowDebugUCLCharacterFallingComponent(
	TEXT("CLShowDebug.CharacterFallingComponent"),
	false,
	TEXT("Shows the Debug information of the CLCharacterFallingComponent class"),
	ECVF_Default);

UCL_CharacterFallingComponent::UCL_CharacterFallingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCL_CharacterFallingComponent::OnMovementModeChanged(ACharacter* Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	if (const EMovementMode CurrentMovementMode = Character->GetCharacterMovement()->MovementMode;
		CurrentMovementMode == MOVE_Falling)
	{
		FallBeginZ = Character->GetActorLocation().Z;
	}
}

void UCL_CharacterFallingComponent::Landed(const FHitResult& Hit)
{
	if (CVarShowDebugUCLCharacterFallingComponent->GetBool())
	{
		UE_LOG(LogCharacterFalling, Display, TEXT("Falling: %f"), FallHeight);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Falling: %f"), FallHeight));
	}

	for (FCL_FallingConditionAndTasks ConditionToTasks : ConditionsToTasks)
	{
		FCL_FallingConditionContext ConditionContext;
		ConditionContext.Character = OwnerCharacter;
		ConditionContext.FallHeight = FallHeight;

		if (ConditionToTasks.Condition.TestCondition(ConditionContext))
		{
			for (const UCL_LandedTaskBase* Task : ConditionToTasks.LandedTasks)
			{
				FCL_LandedTaskContext TaskContext;
				TaskContext.Character = OwnerCharacter;
				TaskContext.HitActor = Hit.GetActor();
				TaskContext.FallHeight = FallHeight;
				TaskContext.FallingComponent = this;
				
				Task->ExecuteTask(TaskContext);
			}
		}
	}
}

//~ Begin UActorComponent

void UCL_CharacterFallingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// We want to always evaluate FallHeight as we allow using it for multiple functionalities 
	if (OwnerCharacter->GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		FallHeight = FallBeginZ - GetOwner()->GetActorLocation().Z;

		// Use reference here because we change the struct's state
		for (FCL_FallingConditionAndTasks& ConditionToTasks : ConditionsToTasks)
		{
			FCL_FallingConditionContext ConditionContext;
			ConditionContext.Character = OwnerCharacter;
			ConditionContext.FallHeight = FallHeight;

			const bool bConditionMet = ConditionToTasks.Condition.TestCondition(ConditionContext);

			switch (ConditionToTasks.State)
			{
			case ECL_FallingRangeState::Outside:
				if (bConditionMet)
				{
					ConditionToTasks.State = ECL_FallingRangeState::Inside;
					for (const UCL_FallingTaskBase* Task : ConditionToTasks.FallingEnterTasks)
					{
						FCL_FallingTaskContext TaskContext;
						TaskContext.Character = OwnerCharacter;
						TaskContext.FallingComponent = this;
				
						Task->ExecuteTask(TaskContext);
					}
				}
				break;
			case ECL_FallingRangeState::Inside:
				if (!bConditionMet)
				{
					ConditionToTasks.State = ECL_FallingRangeState::Outside;
					for (const UCL_FallingTaskBase* Task : ConditionToTasks.FallingExitTasks)
					{
						FCL_FallingTaskContext TaskContext;
						TaskContext.Character = OwnerCharacter;
						TaskContext.FallingComponent = this;
				
						Task->ExecuteTask(TaskContext);
					}
				}
				break;
			default:
				// Should never reach here
				checkNoEntry();
				break;
			}
		}
	}
	else
	{
		// Use reference here because we change the struct's state
		for (FCL_FallingConditionAndTasks& ConditionToTasks : ConditionsToTasks)
		{
			if (ConditionToTasks.State == ECL_FallingRangeState::Inside)
			{
				ConditionToTasks.State = ECL_FallingRangeState::Outside;
				for (const UCL_FallingTaskBase* Task : ConditionToTasks.FallingExitTasks)
				{
					FCL_FallingTaskContext TaskContext;
					TaskContext.Character = OwnerCharacter;
					TaskContext.FallingComponent = this;
				
					Task->ExecuteTask(TaskContext);
				}
			}
		}
	}
}

void UCL_CharacterFallingComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = CastChecked<ACharacter>(GetOwner());
	OwnerCharacter->MovementModeChangedDelegate.AddDynamic(this, &UCL_CharacterFallingComponent::OnMovementModeChanged);
	OwnerCharacter->LandedDelegate.AddDynamic(this, &UCL_CharacterFallingComponent::Landed);
}

//~ End UActorComponent
