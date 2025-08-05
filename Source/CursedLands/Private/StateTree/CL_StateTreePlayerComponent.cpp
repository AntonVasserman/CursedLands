// Copyright Anton Vasserman, All Rights Reserved.


#include "StateTree/CL_StateTreePlayerComponent.h"

#include "StateTreeExecutionContext.h"
#include "StateTree/CL_StateTreePlayerComponentSchema.h"

//~ Begin UStateTreeComponent

TSubclassOf<UStateTreeSchema> UCL_StateTreePlayerComponent::GetSchema() const
{
	return UCL_StateTreePlayerComponentSchema::StaticClass();
}

bool UCL_StateTreePlayerComponent::SetContextRequirements(FStateTreeExecutionContext& Context, bool bLogErrors)
{
	Context.SetCollectExternalDataCallback(FOnCollectStateTreeExternalData::CreateUObject(this, &UCL_StateTreePlayerComponent::CollectExternalData));
	return UCL_StateTreePlayerComponentSchema::SetContextRequirements(*this, Context, bLogErrors);
}

//~ End UStateTreeComponent
