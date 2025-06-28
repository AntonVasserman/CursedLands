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

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsAlive() const;

private:
	void Die() const;
	
	//~ UCL_ResourceComponent Begin
public:
	virtual void InitializeWithAbilitySystem(UCL_AbilitySystemComponent* InAbilitySystemComponent) override;
protected:
	virtual void ResourceStateChanged(ECL_ResourceState OldState, ECL_ResourceState NewState) override;
	//~ UCL_ResourceComponent End
};
