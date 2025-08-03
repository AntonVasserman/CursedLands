// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StateTreeComponentSchema.h"
#include "CL_StateTreePlayerComponentSchema.generated.h"

class ACL_PlayerController;

/*
* State tree schema to be used with CLStateTreePlayerComponent. 
* It guarantees access to an CLPlayerController and the Actor context value can be used to access the controlled pawn.
*/
UCLASS(BlueprintType, EditInlineNew, CollapseCategories, meta = (DisplayName = "StateTree Player Component", CommonSchema))
class CURSEDLANDS_API UCL_StateTreePlayerComponentSchema : public UStateTreeComponentSchema
{
	GENERATED_BODY()
public:
	UCL_StateTreePlayerComponentSchema(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	/** CLPlayerController class the StateTree is expected to run on. Allows binding to specific Actor class' properties. */
	UPROPERTY(EditAnywhere, Category = "Defaults", NoClear)
	TSubclassOf<ACL_PlayerController> PlayerControllerClass = nullptr;

	//~ Begin UStateTreeComponentSchema
public:
	virtual void PostLoad() override;
	virtual void SetContextData(FContextDataSetter& ContextDataSetter, bool bLogErrors) const override;

#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
	
	//~ End UStateTreeComponentSchema
};
