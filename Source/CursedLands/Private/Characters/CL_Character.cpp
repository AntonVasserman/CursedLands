// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/CL_Character.h"

#include "AV_CollisionProfileStatics.h"
#include "CL_GameplayTags.h"
#include "AbilitySystem/CL_AbilitySystemComponent.h"
#include "AbilitySystem/Components/CL_HealthComponent.h"
#include "AbilitySystem/Data/CL_AbilitySet.h"
#include "Characters/Data/CL_PawnData.h"
#include "GameFramework/CharacterMovementComponent.h"

ACL_Character::ACL_Character(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystem = CreateDefaultSubobject<UCL_AbilitySystemComponent>("AbilitySystem");
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	HealthComponent = CreateDefaultSubobject<UCL_HealthComponent>("HealthComponent");
}

bool ACL_Character::IsAlive() const
{
	return HealthComponent->IsAlive();
}

void ACL_Character::SimulatePhysics() const
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(UAV_CollisionProfileStatics::Ragdoll_ProfileName);
}

void ACL_Character::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float Level)
{
	check(IsValid(AbilitySystem));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = AbilitySystem->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystem->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	AbilitySystem->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), AbilitySystem);
}

void ACL_Character::Die()
{
	GetCharacterMovement()->DisableMovement();
	AbilitySystem->RemoveActiveEffects(FGameplayEffectQuery()); // Empty Query to affect all Active Effects
	
	Die_BP();
}

UAnimInstance* ACL_Character::GetAnimInstance()
{
	return GetMesh()->GetAnimInstance();
}

void ACL_Character::SetMovementModeTag(const EMovementMode InMovementMode, const uint8 InCustomMovementMode, const bool bTagEnabled)
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

void ACL_Character::OnGameplayTagNewOrRemoved(FGameplayTag GameplayTag, int NewCount)
{
	if (GameplayTag == CLGameplayTags::Status_Dead)
	{
		if (NewCount > 0)
		{
			Die();
		}
	}
}

//~ ACharacter Begin

void ACL_Character::BeginPlay()
{
	Super::BeginPlay();

	// Check mandatory dependencies
	check(AbilitySystem);

	// Set up Health Component Initialization
	HealthComponent->InitializeWithAbilitySystem(AbilitySystem);
	HealthComponent->InitializeViewModel();
	AbilitySystem->RegisterGameplayTagEvent(CLGameplayTags::Status_Dead, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &ACL_Character::OnGameplayTagNewOrRemoved);
}

void ACL_Character::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	SetMovementModeTag(PrevMovementMode, PreviousCustomMode, false);
	SetMovementModeTag(GetCharacterMovement()->MovementMode, GetCharacterMovement()->CustomMovementMode, true);
}

void ACL_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	check(PawnData);

	for (const UCL_AbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		check(AbilitySet);
		AbilitySet->GiveToAbilitySystem(AbilitySystem, nullptr);
	}
}

void ACL_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AbilitySystem->InitAbilityActorInfo(this, this);
}

//~ ACharacter End

//~ IAbilitySystemInterface Begin

UAbilitySystemComponent* ACL_Character::GetAbilitySystemComponent() const
{
	return GetCLAbilitySystemComponent();
}

//~ IAbilitySystemInterface End

//~ IGameplayTagAssetInterface Begin

void ACL_Character::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (AbilitySystem)
	{
		AbilitySystem->GetOwnedGameplayTags(TagContainer);
	}
}

bool ACL_Character::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	return AbilitySystem ? AbilitySystem->HasMatchingGameplayTag(TagToCheck) : false;
}

bool ACL_Character::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystem ? AbilitySystem->HasAllMatchingGameplayTags(TagContainer) : false;
}

bool ACL_Character::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystem ? AbilitySystem->HasAnyMatchingGameplayTags(TagContainer) : false;
}

//~ IGameplayTagAssetInterface End
