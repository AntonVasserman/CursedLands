// Copyright Anton Vasserman, All Rights Reserved.


#include "StateTree/CL_StateTreePlayerComponent.h"

#include "StateTree/CL_StateTreePlayerComponentSchema.h"

//~ Begin UStateTreeComponent

TSubclassOf<UStateTreeSchema> UCL_StateTreePlayerComponent::GetSchema() const
{
	return UCL_StateTreePlayerComponentSchema::StaticClass();
}

//~ End UStateTreeComponent