// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CL_GameplayAbility.generated.h"

class ACL_Character;

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

	UFUNCTION(BlueprintCallable)
	ACL_Character* GetCLPlayerCharacterFromActorInfo() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config|Ability Activation")
	ECL_AbilityActivationPolicy ActivationPolicy;
};
