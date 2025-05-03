// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Data/CLAbilitySet.h"

#include "ActiveGameplayEffectHandle.h"
#include "CLLogChannels.h"
#include "GameplayAbilitySpecHandle.h"
#include "AbilitySystem/CLAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/CLGameplayAbility.h"
#include "AbilitySystem/Attributes/CLAttributeSet.h"

void FCLAbilitySet_GrantedHandlers::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FCLAbilitySet_GrantedHandlers::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FCLAbilitySet_GrantedHandlers::AddAttributeSet(UCLAttributeSet* Set)
{
	check(Set);
	AttributeSetHandles.Add(Set);
}

void FCLAbilitySet_GrantedHandlers::RemoveFromAbilitySystem(UCLAbilitySystemComponent* ASC)
{
	check(ASC);

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			ASC->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			ASC->RemoveActiveGameplayEffect(Handle);
		}
	}

	for (UCLAttributeSet* Set : AttributeSetHandles)
	{
		ASC->RemoveSpawnedAttribute(Set);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	AttributeSetHandles.Reset();
}

void UCLAbilitySet::GiveToAbilitySystem(UCLAbilitySystemComponent* ASC, FCLAbilitySet_GrantedHandlers* OutGrantedHandlers, UObject* SourceObject) const
{
	check(ASC);

	GiveAttributeSetsToAbilitySystem(ASC, OutGrantedHandlers);
	GiveGameplayAbilitiesToAbilitySystem(ASC, OutGrantedHandlers, SourceObject);
	GiveGameplayEffectsToAbilitySystem(ASC, OutGrantedHandlers);
}

void UCLAbilitySet::GiveAttributeSetsToAbilitySystem(UCLAbilitySystemComponent* ASC, FCLAbilitySet_GrantedHandlers* OutGrantedHandlers) const
{
	for (int32 i = 0; i < GrantedAttributes.Num(); i++)
	{
		const FCLAbilitySet_AttributeSet& SetToGrant = GrantedAttributes[i];

		if (!IsValid(SetToGrant.AttributeSet))
		{
			UE_LOG(LogCLGameplayAbilitySystem, Error, TEXT("GrantedAttributes[%d] on ability set [%s] is not valid"), i, *GetNameSafe(this));
			continue;
		}

		UCLAttributeSet* NewSet = NewObject<UCLAttributeSet>(ASC->GetOwner(), SetToGrant.AttributeSet);
		ASC->AddAttributeSetSubobject(NewSet);

		if (OutGrantedHandlers)
		{
			OutGrantedHandlers->AddAttributeSet(NewSet);
		}
	}
}

void UCLAbilitySet::GiveGameplayAbilitiesToAbilitySystem(UCLAbilitySystemComponent* ASC, FCLAbilitySet_GrantedHandlers* OutGrantedHandlers, UObject* SourceObject) const
{
	for (int32 i = 0; i < GrantedGameplayAbilities.Num(); i++)
	{
		const FCLAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[i];

		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogCLGameplayAbilitySystem, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid"), i, *GetNameSafe(this));
			continue;
		}

		UCLGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UCLGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = ASC->GiveAbility(AbilitySpec);

		if (OutGrantedHandlers)
		{
			OutGrantedHandlers->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}
}

void UCLAbilitySet::GiveGameplayEffectsToAbilitySystem(UCLAbilitySystemComponent* ASC, FCLAbilitySet_GrantedHandlers* OutGrantedHandlers) const
{
	for (int32 i = 0; i < GrantedGameplayEffects.Num(); i++)
	{
		const FCLAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[i];

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			UE_LOG(LogCLGameplayAbilitySystem, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid"), i, *GetNameSafe(this));
			continue;
		}

		const UGameplayEffect* GameplayEffectCDO = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle GameplayEffectHandle = ASC->ApplyGameplayEffectToSelf(GameplayEffectCDO, EffectToGrant.EffectLevel, ASC->MakeEffectContext());

		if (OutGrantedHandlers)
		{
			OutGrantedHandlers->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}
}
