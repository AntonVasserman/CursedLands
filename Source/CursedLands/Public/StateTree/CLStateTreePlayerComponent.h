// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StateTreeComponent.h"
#include "CLStateTreePlayerComponent.generated.h"

/**
* State tree component designed to be run on a CLPlayerController.
* It uses the CLStateTreePlayerComponentSchema that guarantees access to the CLPlayerController.
*/
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class CURSEDLANDS_API UCLStateTreePlayerComponent : public UStateTreeComponent
{
	GENERATED_BODY()
public:
	//~ Begin UStateTreeComponent
	virtual TSubclassOf<UStateTreeSchema> GetSchema() const override;
	//~ End UStateTreeComponent
};
