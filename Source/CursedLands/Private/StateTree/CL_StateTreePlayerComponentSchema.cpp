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
	ContextDataDescs.Emplace(UE::GameplayStateTree::Private::Name_CLPlayerController, PlayerControllerClass.Get(), FGuid(0xA24EA97C, 0xD9E24BAC, 0xB41A4A8F, 0x63EA36FD));
}

//~ Begin UStateTreeSchema

void UCL_StateTreePlayerComponentSchema::PostLoad()
{
	Super::PostLoad();
	ContextDataDescs[1].Struct = PlayerControllerClass.Get();
}

void UCL_StateTreePlayerComponentSchema::SetContextData(FContextDataSetter& ContextDataSetter, bool bLogErrors) const
{
	const FName PlayerControllerName = UE::GameplayStateTree::Private::Name_CLPlayerController;

	const UBrainComponent* BrainComponent = ContextDataSetter.GetComponent();
	checkf(BrainComponent, TEXT("Failed to get BrainComponent from ContextDataSetter"));

	const APawn* Pawn = Cast<APawn>(BrainComponent->GetOwner());
	checkf(Pawn, TEXT("Failed to get Pawn from BrainComponent"));

	APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());
	checkf(PlayerController, TEXT("Failed to get PlayerController from Pawn"));

	// TODO (CL-130): Fix the LNK2019 and LNK1120 this generates:
	// ContextDataSetter.SetContextDataByName(PlayerControllerName, FStateTreeDataView(PlayerController));
	
	Super::SetContextData(ContextDataSetter, bLogErrors);
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
