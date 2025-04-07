// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/CLCharacter.h"

#include "AbilitySystem/CLAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/CLAttributeSet.h"
#include "AbilitySystem/Attributes/CLHealthAttributeSet.h"

ACLCharacter::ACLCharacter()
{
	AbilitySystem = CreateDefaultSubobject<UCLAbilitySystemComponent>("AbilitySystem");
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	HealthAttributeSet = CreateDefaultSubobject<UCLHealthAttributeSet>("HealthAttributeSet");
}

bool ACLCharacter::AddUniqueGameplayTag(const FGameplayTag& GameplayTag)
{
	if (HasMatchingGameplayTag(GameplayTag))
	{
		return false;
	}
	
	AbilitySystem->AddLooseGameplayTag(GameplayTag);
	return true;
}

void ACLCharacter::RemoveGameplayTag(const FGameplayTag& GameplayTag)
{
	AbilitySystem->RemoveLooseGameplayTag(GameplayTag);
}

void ACLCharacter::InitAbilityActorInfo()
{
	AbilitySystem->InitAbilityActorInfo(this, this);

	// TODO: Is this the correct place?
	// This currently means each time we "Possess" the character by a controller it will reset his values...
	InitializeDefaultAttributes();
	InitializeDefaultPassiveEffects();
}

void ACLCharacter::SimulatePhysics() const
{
	GetMesh()->SetSimulatePhysics(true);
	// TODO: Consider a const here, or even better, use my common logic plugin
	GetMesh()->SetCollisionProfileName("Ragdoll");
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
	AbilitySystem->RemoveActiveEffects(FGameplayEffectQuery()); // Empty Query to affect all Active Effects
	bIsAlive = false;
	Die_BP();
}

void ACLCharacter::InitializeDefaultAttributes()
{
	ApplyEffectToSelf(DefaultGeneralAttributesMaxValueEffectClass, 1.0);
	ApplyEffectToSelf(DefaultGeneralAttributesValueEffectClass, 1.0);
}

void ACLCharacter::InitializeDefaultPassiveEffects()
{
	for (TSubclassOf<UGameplayEffect> PassiveEffectClass : DefaultPassiveEffectClasses)
	{
		ApplyEffectToSelf(PassiveEffectClass, 1.0);
	}
}

//~ ACharacter Begin

void ACLCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AbilitySystem->GetGameplayAttributeValueChangeDelegate(GetHealthAttributeSet()->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			if (Data.NewValue == 0 && IsAlive())
			{
				Die();
			}
		});
}

void ACLCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	InitAbilityActorInfo();
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
