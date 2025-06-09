// Copyright Anton Vasserman, All Rights Reserved.


#include "StateTree/CLStateTreePlayerComponent.h"

#include "StateTree/CLStateTreePlayerComponentSchema.h"

//~ Begin UStateTreeComponent

TSubclassOf<UStateTreeSchema> UCLStateTreePlayerComponent::GetSchema() const
{
	return UCLStateTreePlayerComponentSchema::StaticClass();
}

//~ End UStateTreeComponent