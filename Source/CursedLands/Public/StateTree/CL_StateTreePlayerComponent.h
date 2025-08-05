// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StateTreeComponent.h"
#include "CL_StateTreePlayerComponent.generated.h"

/**
* State tree component designed to be run on a CLPlayerController.
* It uses the CLStateTreePlayerComponentSchema that guarantees access to the CLPlayerController.
*/
UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class CURSEDLANDS_API UCL_StateTreePlayerComponent : public UStateTreeComponent
{
	GENERATED_BODY()
public:
	//~ Begin UStateTreeComponent
	virtual TSubclassOf<UStateTreeSchema> GetSchema() const override;
	virtual bool SetContextRequirements(FStateTreeExecutionContext& Context, bool bLogErrors = false) override;
	//~ End UStateTreeComponent
};
