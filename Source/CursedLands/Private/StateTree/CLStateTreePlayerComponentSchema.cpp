// Copyright Anton Vasserman, All Rights Reserved.


#include "StateTree/CLStateTreePlayerComponentSchema.h"

#include "StateTreeExecutionContext.h"
#include "Controllers/CLPlayerController.h"
#include "GameFramework/Pawn.h"

namespace UE::GameplayStateTree::Private
{
	static FLazyName Name_CLPlayerController = "CLPlayerController";
}

UCLStateTreePlayerComponentSchema::UCLStateTreePlayerComponentSchema(const FObjectInitializer& ObjectInitializer)
	: PlayerControllerClass(ACLPlayerController::StaticClass())
{
	check(ContextDataDescs.Num() == 1 && ContextDataDescs[0].Struct == AActor::StaticClass());
	// Make the Actor a pawn by default so it bind to the controlled pawn instead of the CLPlayerController.
	ContextActorClass = APawn::StaticClass();
	ContextDataDescs[0].Struct = ContextActorClass.Get();
	ContextDataDescs.Emplace(UE::GameplayStateTree::Private::Name_CLPlayerController, PlayerControllerClass.Get(), FGuid(0xA24EA97C, 0xD9E24BAC, 0xB41A4A8F, 0x63EA36FD));
}

//~ Begin UStateTreeSchema

void UCLStateTreePlayerComponentSchema::PostLoad()
{
	Super::PostLoad();
	ContextDataDescs[1].Struct = PlayerControllerClass.Get();
}

#if WITH_EDITOR
void UCLStateTreePlayerComponentSchema::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	if (const FProperty* Property = PropertyChangedEvent.Property)
	{
		if (Property->GetOwnerClass() == UCLStateTreePlayerComponentSchema::StaticClass()
			&& Property->GetFName() == GET_MEMBER_NAME_CHECKED(UCLStateTreePlayerComponentSchema, PlayerControllerClass))
		{
			ContextDataDescs[1].Struct = PlayerControllerClass.Get();
		}
	}
}
#endif // WITH_EDITOR

//~ End UStateTreeSchema
