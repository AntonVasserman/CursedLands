// Copyright Anton Vasserman, All Rights Reserved.


#include "Core/Characters/CLCharacter.h"

#include "AbilitySystem/CLAbilitySystemComponent.h"
#include "AbilitySystem/CLAttributeSet.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Controller.h"

ACLCharacter::ACLCharacter()
{
	AbilitySystem = CreateDefaultSubobject<UCLAbilitySystemComponent>("AbilitySystem");
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	AttributeSet = CreateDefaultSubobject<UCLAttributeSet>("AttributeSet");
}

void ACLCharacter::InitAbilityActorInfo()
{
	AbilitySystem->InitAbilityActorInfo(this, this);

	// TODO: Is this the correct place?
	// This currently means each time we "Possess" the character by a controller it will reset his values...
	InitializeDefaultAttributes();
}

void ACLCharacter::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float Level) const
{
	check(IsValid(AbilitySystem));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = AbilitySystem->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystem->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	AbilitySystem->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), AbilitySystem);
}

void ACLCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.0);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.0);
}

//~ ACharacter Begin

void ACLCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	InitAbilityActorInfo();
}

//~ ACharacter End
