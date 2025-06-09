// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StateTreeComponentSchema.h"
#include "CLStateTreePlayerComponentSchema.generated.h"

class ACLPlayerController;

/*
* State tree schema to be used with CLStateTreePlayerComponent. 
* It guarantees access to an CLPlayerController and the Actor context value can be used to access the controlled pawn.
*/
UCLASS(BlueprintType, EditInlineNew, CollapseCategories, meta = (DisplayName = "StateTree Player Component", CommonSchema))
class CURSEDLANDS_API UCLStateTreePlayerComponentSchema : public UStateTreeComponentSchema
{
	GENERATED_BODY()
public:
	UCLStateTreePlayerComponentSchema(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	/** CLPlayerController class the StateTree is expected to run on. Allows binding to specific Actor class' properties. */
	UPROPERTY(EditAnywhere, Category = "Defaults", NoClear)
	TSubclassOf<ACLPlayerController> PlayerControllerClass = nullptr;

	//~ Begin UStateTreeComponentSchema
public:
	virtual void PostLoad() override;

#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
	
	//~ End UStateTreeComponentSchema
};
