// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CL_GameplayAbility.h"
#include "CL_GameplayAbility_Sprint.generated.h"

UCLASS()
class CURSEDLANDS_API UCL_GameplayAbility_Sprint : public UCL_GameplayAbility
{
	GENERATED_BODY()

public:
	UCL_GameplayAbility_Sprint(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) { }

	//~ Begin UCL_GameplayAbility
public:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	//~ End UCL_GameplayAbility
};
