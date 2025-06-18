// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/CLCharacter.h"

#include "AVCollisionProfileStatics.h"
#include "CL_GameplayTags.h"
#include "AbilitySystem/CL_AbilitySystemComponent.h"
#include "AbilitySystem/Components/CL_HealthComponent.h"
#include "AbilitySystem/Data/CL_AbilitySet.h"
#include "Characters/Data/CLPawnData.h"
#include "GameFramework/CharacterMovementComponent.h"

ACLCharacter::ACLCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystem = CreateDefaultSubobject<UCL_AbilitySystemComponent>("AbilitySystem");
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	HealthComponent = CreateDefaultSubobject<UCL_HealthComponent>("HealthComponent");
}

bool ACLCharacter::IsAlive() const
{
	TOptional<bool> IsAliveResult = HealthComponent->IsAlive();
	return IsAliveResult.IsSet() && IsAliveResult.GetValue();
}

void ACLCharacter::SimulatePhysics() const
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(UAVCollisionProfileStatics::Ragdoll_ProfileName);
}

void ACLCharacter::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float Level)
{
	check(IsValid(AbilitySystem));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = AbilitySystem->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystem->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	AbilitySystem->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), AbilitySystem);
}

void ACLCharacter::Die()
{
	GetCharacterMovement()->DisableMovement();
	AbilitySystem->RemoveActiveEffects(FGameplayEffectQuery()); // Empty Query to affect all Active Effects
	
	Die_BP();
}

UAnimInstance* ACLCharacter::GetAnimInstance()
{
	return GetMesh()->GetAnimInstance();
}

void ACLCharacter::SetMovementModeTag(const EMovementMode InMovementMode, const uint8 InCustomMovementMode, const bool bTagEnabled)
{
	if (AbilitySystem)
	{
		const FGameplayTag* MovementModeTag = InMovementMode == MOVE_Custom ?
			CLGameplayTags::CustomMovementModeTagMap.Find(InCustomMovementMode) :
			CLGameplayTags::MovementModeTagMap.Find(InMovementMode);

		if (MovementModeTag && MovementModeTag->IsValid())
		{
			AbilitySystem->SetLooseGameplayTagCount(*MovementModeTag, bTagEnabled ? 1 : 0);
		}
	}
}

//~ ACharacter Begin

void ACLCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Check mandatory dependencies
	check(AbilitySystem);

	// Set up Health Component Initialization
	HealthComponent->InitializeWithAbilitySystem(AbilitySystem);
	HealthComponent->OnDied.AddDynamic(this, &ACLCharacter::Die);
}

void ACLCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	SetMovementModeTag(PrevMovementMode, PreviousCustomMode, false);
	SetMovementModeTag(GetCharacterMovement()->MovementMode, GetCharacterMovement()->CustomMovementMode, true);
}

void ACLCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	check(PawnData);

	for (const UCL_AbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		check(AbilitySet);
		AbilitySet->GiveToAbilitySystem(AbilitySystem, nullptr);
	}
}

void ACLCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AbilitySystem->InitAbilityActorInfo(this, this);
}

//~ ACharacter End

//~ IAbilitySystemInterface Begin

UAbilitySystemComponent* ACLCharacter::GetAbilitySystemComponent() const
{
	return GetCLAbilitySystemComponent();
}

//~ IAbilitySystemInterface End

//~ IGameplayTagAssetInterface Begin

void ACLCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (AbilitySystem)
	{
		AbilitySystem->GetOwnedGameplayTags(TagContainer);
	}
}

bool ACLCharacter::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	return AbilitySystem ? AbilitySystem->HasMatchingGameplayTag(TagToCheck) : false;
}

bool ACLCharacter::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystem ? AbilitySystem->HasAllMatchingGameplayTags(TagContainer) : false;
}

bool ACLCharacter::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystem ? AbilitySystem->HasAnyMatchingGameplayTags(TagContainer) : false;
}

//~ IGameplayTagAssetInterface End
