// Copyright Anton Vasserman, All Rights Reserved.


#include "Characters/Gameplay/AbilitySystem/Abilities/CL_GameplayAbility_Slide.h"

#include "Characters/CL_PlayerCharacter.h"

void UCL_GameplayAbility_Slide::OnTraversalActionFinished(const ECL_TraversalAction TraversalAction)
{
	GetCLPlayerCharacterFromActorInfo(CachedActorInfo)->GetCharacterTraversal()->OnTraversalActionFinished.RemoveDynamic(this, &UCL_GameplayAbility_Slide::OnTraversalActionFinished);
	EndAbility(CachedHandle, CachedActorInfo, CachedActivationInfo, false, false);
}

//~ Begin UCL_GameplayAbility_PlayerCharacterBase

bool UCL_GameplayAbility_Slide::CanActivateAbilityInternal(const ACL_PlayerCharacter* PlayerCharacter, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return PlayerCharacter->CanSlide();
}

void UCL_GameplayAbility_Slide::ActivateAbilityInternal(ACL_PlayerCharacter* PlayerCharacter, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	CachedHandle = Handle;
	CachedActorInfo = ActorInfo;
	CachedActivationInfo = ActivationInfo;
	PlayerCharacter->GetCharacterTraversal()->OnTraversalActionFinished.AddDynamic(this, &UCL_GameplayAbility_Slide::OnTraversalActionFinished);
	
	PlayerCharacter->Slide();
}

//~ End UCL_GameplayAbility_PlayerCharacterBase 
