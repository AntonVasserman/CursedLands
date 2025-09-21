// Copyright Anton Vasserman, All Rights Reserved.


#include "Pawn/Enemy/CL_EnemyPawn.h"

#include "CL_GameplayTags.h"
#include "AbilitySystem/CL_AbilitySystemComponent.h"
#include "AbilitySystem/Components/CL_HealthComponent.h"
#include "AbilitySystem/Data/CL_AbilitySet.h"
#include "Characters/Data/CL_PawnData.h"
#include "UI/HUD/Data/CL_ResourceGameplayTagsData.h"

ACL_EnemyPawn::ACL_EnemyPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AbilitySystem = CreateDefaultSubobject<UCL_AbilitySystemComponent>("AbilitySystem");
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	HealthComponent = CreateDefaultSubobject<UCL_HealthComponent>("HealthComponent");
}

void ACL_EnemyPawn::Die()
{
	AbilitySystem->RemoveActiveEffects(FGameplayEffectQuery()); // Empty Query to affect all Active Effects
	
	Die_BP();
}

void ACL_EnemyPawn::OnGameplayTagNewOrRemoved(FGameplayTag GameplayTag, int NewCount)
{
	if (GameplayTag == CLGameplayTags::Status_Dead)
	{
		if (NewCount > 0)
		{
			Die();
		}
	}
}

//~ ACL_Pawn Begin
void ACL_EnemyPawn::BeginPlay()
{
	Super::BeginPlay();

	check(PawnData);
	
	for (const UCL_AbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		check(AbilitySet);
		AbilitySet->GiveToAbilitySystem(AbilitySystem, nullptr);
	}

	// Set up Health Component Initialization
	HealthComponent->InitializeWithAbilitySystem(AbilitySystem);

	AbilitySystem->RegisterGameplayTagEvent(CLGameplayTags::Status_Dead, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &ThisClass::OnGameplayTagNewOrRemoved);
}

void ACL_EnemyPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AbilitySystem->InitAbilityActorInfo(this, this);
}

//~ ACL_Pawn End
