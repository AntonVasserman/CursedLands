// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_GameplayAbility.h"
#include "CL_GameplayAbility_Death.generated.h"

UCLASS(Abstract)
class CURSEDLANDS_API UCL_GameplayAbility_Death : public UCL_GameplayAbility
{
	GENERATED_BODY()

public:
	UCL_GameplayAbility_Death(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~ Begin UCL_GameplayAbility
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UCL_GameplayAbility
};
