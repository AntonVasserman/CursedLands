// Copyright Anton Vasserman, All Rights Reserved.


#include "StateTree/CL_StateTreePlayerComponentSchema.h"

#include "BrainComponent.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeTypes.h"
#include "Controllers/CL_PlayerController.h"
#include "GameFramework/Pawn.h"

namespace UE::GameplayStateTree::Private
{
	static FLazyName Name_CLPlayerController = "CLPlayerController";
}

UCL_StateTreePlayerComponentSchema::UCL_StateTreePlayerComponentSchema(const FObjectInitializer& ObjectInitializer)
	: PlayerControllerClass(ACL_PlayerController::StaticClass())
{
	check(ContextDataDescs.Num() == 1 && ContextDataDescs[0].Struct == AActor::StaticClass());
	// Make the Actor a pawn by default so it bind to the controlled pawn instead of the CLPlayerController.
	ContextActorClass = APawn::StaticClass();
	ContextDataDescs[0].Struct = ContextActorClass.Get();
	ContextDataDescs.Emplace(UE::GameplayStateTree::Private::Name_CLPlayerController, PlayerControllerClass.Get(), FGuid::NewGuid());
}

//~ Begin UStateTreeSchema

void UCL_StateTreePlayerComponentSchema::PostLoad()
{
	Super::PostLoad();
	ContextDataDescs[1].Struct = PlayerControllerClass.Get();
}

bool UCL_StateTreePlayerComponentSchema::SetContextRequirements(UBrainComponent& BrainComponent, FStateTreeExecutionContext& Context, bool bLogErrors)
{
	const FName PlayerControllerName = UE::GameplayStateTree::Private::Name_CLPlayerController;

	// Here we basically strict this StateTree to only be applied to Pawns, not controllers
	const APawn* Pawn = Cast<APawn>(BrainComponent.GetOwner());
	checkf(Pawn, TEXT("Failed to get Pawn from BrainComponent"));

	// The Controller has the potential of being null upon closing the game, so we don't check here, just use an if
	if (ACL_PlayerController* PlayerController = Cast<ACL_PlayerController>(Pawn->GetController());
		PlayerController != nullptr)
	{
		Context.SetContextDataByName(PlayerControllerName, FStateTreeDataView(PlayerController));
	}
	
	return Super::SetContextRequirements(BrainComponent, Context, bLogErrors);
}

#if WITH_EDITOR
void UCL_StateTreePlayerComponentSchema::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	if (const FProperty* Property = PropertyChangedEvent.Property)
	{
		if (Property->GetOwnerClass() == StaticClass()
			&& Property->GetFName() == GET_MEMBER_NAME_CHECKED(UCL_StateTreePlayerComponentSchema, PlayerControllerClass))
		{
			ContextDataDescs[1].Struct = PlayerControllerClass.Get();
		}
	}
}
#endif // WITH_EDITOR

//~ End UStateTreeSchema
