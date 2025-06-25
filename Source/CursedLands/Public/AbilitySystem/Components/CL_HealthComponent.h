// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CL_ResourceComponent.h"
#include "CL_HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCL_OnDied);

UCLASS()
class CURSEDLANDS_API UCL_HealthComponent : public UCL_ResourceComponent
{
	GENERATED_BODY()

public:
	UCL_HealthComponent();

	FCL_OnDied OnDied;

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsAlive() const;

private:
	void Die() const;
	
	//~ UCL_ResourceComponent Begin
public:
	virtual void InitializeWithAbilitySystem(UCL_AbilitySystemComponent* InAbilitySystemComponent) override;

protected:
	virtual void ResourceDepleted() override;
	virtual void ResourceFull() override;
	//~ UCL_ResourceComponent End
};
