// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_ResourceComponent.h"
#include "CL_StaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCL_OnFatigueApplied);

UCLASS()
class CURSEDLANDS_API UCL_StaminaComponent : public UCL_ResourceComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "")
	uint8 bApplyFatigueOnStaminaDepleted : 1 = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (EditCondition = "bApplyFatigueOnStaminaDepleted == true", EditConditionHides))
	TSubclassOf<UGameplayEffect> FatigueGameplayEffectClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (EditCondition = "bApplyFatigueOnStaminaDepleted == true && FatigueGameplayEffectClass != nullptr", EditConditionHides))
	float FatigueGameplayEffectLevel = 1.f;

public:
	UCL_StaminaComponent();

	UPROPERTY(BlueprintAssignable, Category = "Stamina")
	FCL_OnFatigueApplied OnFatigueApplied;
	
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	bool IsFatigued() const;
	
private:
	void ApplyFatigue() const;
	
	//~ UCL_ResourceComponent Begin
protected:
	virtual void ResourceDepleted() override;
	virtual void ResourceFull() override;
	//~ UCL_ResourceComponent End
};
