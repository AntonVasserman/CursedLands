// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CL_GameplayAbility.generated.h"

class ACL_Character;
class ACL_PlayerCharacter;

UENUM(BlueprintType)
enum class ECL_AbilityActivationPolicy : uint8
{
	OnInputTriggered,
	WhileInputActive,
	OnSpawn
};

UCLASS()
class CURSEDLANDS_API UCL_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	ECL_AbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }
	ACL_Character* GetCLCharacterFromActorInfo(const FGameplayAbilityActorInfo* ActorInfo) const;
	ACL_PlayerCharacter* GetCLPlayerCharacterFromActorInfo(const FGameplayAbilityActorInfo* ActorInfo) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Ability Activation")
	ECL_AbilityActivationPolicy ActivationPolicy;
};
