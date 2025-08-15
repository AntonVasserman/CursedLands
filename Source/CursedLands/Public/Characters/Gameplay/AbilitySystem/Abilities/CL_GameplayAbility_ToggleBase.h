// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CL_GameplayAbility.h"
#include "CL_GameplayAbility_ToggleBase.generated.h"

class ACL_PlayerCharacter;

UCLASS(Abstract)
class CURSEDLANDS_API UCL_GameplayAbility_ToggleBase : public UCL_GameplayAbility
{
	GENERATED_BODY()

public:
	UCL_GameplayAbility_ToggleBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) { }

protected:
	virtual bool IsToggled(const ACL_PlayerCharacter* PlayerCharacter) const { unimplemented(); return false; }
	virtual bool CanToggle(const ACL_PlayerCharacter* PlayerCharacter) const { unimplemented(); return false; }
	virtual bool CanUnToggle(const ACL_PlayerCharacter* PlayerCharacter) const { unimplemented(); return false; }
	virtual void Toggle(ACL_PlayerCharacter* PlayerCharacter) { unimplemented(); }
	void virtual UnToggle(ACL_PlayerCharacter* PlayerCharacter) { unimplemented(); }
	
	//~ Begin UCL_GameplayAbility
public:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	//~ End UCL_GameplayAbility
};
