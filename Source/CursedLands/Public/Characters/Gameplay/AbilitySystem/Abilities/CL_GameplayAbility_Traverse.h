// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_GameplayAbility_PlayerCharacterBase.h"
#include "AbilitySystem/Abilities/CL_GameplayAbility.h"
#include "CL_GameplayAbility_Traverse.generated.h"

UCLASS(Abstract)
class CURSEDLANDS_API UCL_GameplayAbility_Traverse : public UCL_GameplayAbility_PlayerCharacterBase
{
	GENERATED_BODY()

	//~ Begin UCL_GameplayAbility_PlayerCharacterBase
protected:
	virtual bool CanActivateAbilityInternal(const ACL_PlayerCharacter* PlayerCharacter, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbilityInternal(ACL_PlayerCharacter* PlayerCharacter, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	//~ End UCL_GameplayAbility_PlayerCharacterBase
};
