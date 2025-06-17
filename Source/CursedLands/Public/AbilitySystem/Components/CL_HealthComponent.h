// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_ResourceComponent.h"
#include "CL_HealthComponent.generated.h"

UCLASS()
class CURSEDLANDS_API UCL_HealthComponent : public UCL_ResourceComponent
{
	GENERATED_BODY()

public:
	UCL_HealthComponent();

	UFUNCTION(BlueprintCallable)
	TOptional<bool> IsAlive() const;

	//~ UCL_ResourceComponent Begin
	virtual void InitializeWithAbilitySystem(UCL_AbilitySystemComponent* InAbilitySystemComponent) override;

protected:
	virtual void ResourceDepletedInternal() override;
	virtual void ResourceFullInternal() override;
	//~ UCL_ResourceComponent End
};
