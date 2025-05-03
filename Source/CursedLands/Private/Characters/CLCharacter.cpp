// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/CLCharacter.h"

#include "AVCollisionProfileStatics.h"
#include "CLGameplayTags.h"
#include "AbilitySystem/CLAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/CLAttributeSet.h"
#include "AbilitySystem/Attributes/CLHealthAttributeSet.h"
#include "AbilitySystem/Data/CLAbilitySet.h"
#include "Characters/Data/CLPawnData.h"
#include "GameFramework/CharacterMovementComponent.h"

ACLCharacter::ACLCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystem = CreateDefaultSubobject<UCLAbilitySystemComponent>("AbilitySystem");
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	HealthAttributeSet = CreateDefaultSubobject<UCLHealthAttributeSet>("HealthAttributeSet");
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
	bIsAlive = false;
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

	for (const UCLAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		check(AbilitySet);
		AbilitySet->GiveToAbilitySystem(AbilitySystem, nullptr);
	}
}

void ACLCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AbilitySystem->InitAbilityActorInfo(this, this);
	AbilitySystem->GetGameplayAttributeValueChangeDelegate(GetHealthAttributeSet()->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			if (Data.NewValue == 0 && IsAlive())
			{
				Die();
			}
		});
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
