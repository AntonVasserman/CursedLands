// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_GASOperationBase.h"
#include "GameplayEffect.h"
#include "CL_ApplyGameplayEffectOperation.generated.h"

UCLASS()
class CURSEDLANDS_API UCL_ApplyGameplayEffectOperation : public UCL_GASOperationBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "")
	TSubclassOf<UGameplayEffect> GameplayEffectClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "", Meta = (EditCondition = "GameplayEffectClass != nullptr", EditConditionHides))
	float GameplayEffectLevel = 1.f;
	
	//~ UCL_GASOperationBase Begin
public:
	virtual void Execute_Implementation(UAbilitySystemComponent* InAbilitySystemComponent) const override;
	virtual FString GetOperationName_Implementation() const override;
	//~ UCL_GASOperationBase End
};
