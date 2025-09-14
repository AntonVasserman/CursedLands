// Copyright Anton Vasserman, All Rights Reserved.


#include "AbilitySystem/Data/CL_AbilitySet.h"

#include "ActiveGameplayEffectHandle.h"
#include "CL_LogChannels.h"
#include "GameplayAbilitySpecHandle.h"
#include "AbilitySystem/CL_AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/CL_GameplayAbility.h"
#include "AbilitySystem/Attributes/CL_AttributeSet.h"

void FCL_AbilitySet_GrantedHandlers::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FCL_AbilitySet_GrantedHandlers::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FCL_AbilitySet_GrantedHandlers::AddAttributeSet(UCL_AttributeSet* Set)
{
	check(Set);
	AttributeSetHandles.Add(Set);
}

void FCL_AbilitySet_GrantedHandlers::RemoveFromAbilitySystem(UCL_AbilitySystemComponent* ASC)
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

	for (UCL_AttributeSet* Set : AttributeSetHandles)
	{
		ASC->RemoveSpawnedAttribute(Set);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	AttributeSetHandles.Reset();
}

void UCL_AbilitySet::GiveToAbilitySystem(UCL_AbilitySystemComponent* ASC, FCL_AbilitySet_GrantedHandlers* OutGrantedHandlers, UObject* SourceObject) const
{
	check(ASC);

	GiveAttributeSetsToAbilitySystem(ASC, OutGrantedHandlers);
	GiveGameplayAbilitiesToAbilitySystem(ASC, OutGrantedHandlers, SourceObject);
	GiveGameplayEffectsToAbilitySystem(ASC, OutGrantedHandlers);
}

void UCL_AbilitySet::GiveAttributeSetsToAbilitySystem(UCL_AbilitySystemComponent* ASC, FCL_AbilitySet_GrantedHandlers* OutGrantedHandlers) const
{
	for (int32 i = 0; i < GrantedAttributes.Num(); i++)
	{
		const FCL_AbilitySet_AttributeSet& SetToGrant = GrantedAttributes[i];

		if (!IsValid(SetToGrant.AttributeSet))
		{
			CL_LOG_GAMEPLAY_ABILITY_SYSTEM(Error, "GrantedAttributes[%d] on ability set [%s] is not valid", i, *GetNameSafe(this));
			continue;
		}

		UCL_AttributeSet* NewSet = NewObject<UCL_AttributeSet>(ASC->GetOwner(), SetToGrant.AttributeSet);
		ASC->AddAttributeSetSubobject(NewSet);

		if (OutGrantedHandlers)
		{
			OutGrantedHandlers->AddAttributeSet(NewSet);
		}
	}
}

void UCL_AbilitySet::GiveGameplayAbilitiesToAbilitySystem(UCL_AbilitySystemComponent* ASC, FCL_AbilitySet_GrantedHandlers* OutGrantedHandlers, UObject* SourceObject) const
{
	for (int32 i = 0; i < GrantedGameplayAbilities.Num(); i++)
	{
		const FCL_AbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[i];

		if (!IsValid(AbilityToGrant.Ability))
		{
			CL_LOG_GAMEPLAY_ABILITY_SYSTEM(Error, "GrantedGameplayAbilities[%d] on ability set [%s] is not valid", i, *GetNameSafe(this));
			continue;
		}
		
		UCL_GameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UCL_GameplayAbility>();

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

void UCL_AbilitySet::GiveGameplayEffectsToAbilitySystem(UCL_AbilitySystemComponent* ASC, FCL_AbilitySet_GrantedHandlers* OutGrantedHandlers) const
{
	for (int32 i = 0; i < GrantedGameplayEffects.Num(); i++)
	{
		const FCL_AbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[i];

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			CL_LOG_GAMEPLAY_ABILITY_SYSTEM(Error, "GrantedGameplayEffects[%d] on ability set [%s] is not valid", i, *GetNameSafe(this));
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
