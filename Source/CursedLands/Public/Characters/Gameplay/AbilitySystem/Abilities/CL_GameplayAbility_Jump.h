// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_GameplayAbility_PlayerCharacterBase.h"
#include "AbilitySystem/Abilities/CL_GameplayAbility.h"
#include "CL_GameplayAbility_Jump.generated.h"

UCLASS(Abstract)
class CURSEDLANDS_API UCL_GameplayAbility_Jump : public UCL_GameplayAbility_PlayerCharacterBase
{
	GENERATED_BODY()

public:
	UCL_GameplayAbility_Jump(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) { }

	//~ Begin UCL_GameplayAbility_PlayerCharacterBase
public:
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
protected:
	virtual bool CanActivateAbilityInternal(const ACL_PlayerCharacter* PlayerCharacter, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbilityInternal(ACL_PlayerCharacter* PlayerCharacter, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	//~ End UCL_GameplayAbility_PlayerCharacterBase
};
