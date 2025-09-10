// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_GameplayAbility_PlayerCharacterBase.h"
#include "AbilitySystem/Abilities/CL_GameplayAbility.h"
#include "Systems/Traversal/CL_CharacterTraversalComponent.h"
#include "CL_GameplayAbility_Slide.generated.h"

UCLASS()
class CURSEDLANDS_API UCL_GameplayAbility_Slide : public UCL_GameplayAbility_PlayerCharacterBase
{
	GENERATED_BODY()

public:
	UCL_GameplayAbility_Slide(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) { }

private:
	FGameplayAbilitySpecHandle CachedHandle;
	const FGameplayAbilityActorInfo* CachedActorInfo;
	FGameplayAbilityActivationInfo CachedActivationInfo;
	
	UFUNCTION()
	void OnTraversalActionFinished(const ECL_TraversalAction TraversalAction);
	
	//~ Begin UCL_GameplayAbility_PlayerCharacterBase 
protected:
	virtual bool CanActivateAbilityInternal(const ACL_PlayerCharacter* PlayerCharacter, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbilityInternal(ACL_PlayerCharacter* PlayerCharacter, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	//~ End UCL_GameplayAbility_PlayerCharacterBase
};
