// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "UI/Foundation/Confirmation/CL_ConfirmationWidget.h"
#include "CL_AsyncAction_ShowConfirmation.generated.h"

class UCL_ConfirmationDescriptor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCL_ConfirmationResultMCDelegate, ECL_ConfirmationWidgetResult, Result);

// TODO (CL-268): Consider moving instead to CommonGame Lyra Plugin
UCLASS()
class CURSEDLANDS_API UCL_AsyncAction_ShowConfirmation : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Meta = (BlueprintInternalUseOnly = "true", WorldContext = "InWorldContextObject"))
	static UCL_AsyncAction_ShowConfirmation* ShowConfirmationYesNo(UObject* InWorldContextObject, TSubclassOf<UCL_ConfirmationWidget> InConfirmationWidgetClass, FText Title, FText Message);

	UPROPERTY(BlueprintAssignable)
	FCL_ConfirmationResultMCDelegate OnResult;

private:
	void HandleConfirmationResult(ECL_ConfirmationWidgetResult ConfirmationResult);
	
	UPROPERTY(Transient)
	TObjectPtr<UObject> WorldContextObject;

	UPROPERTY(Transient)
	TSubclassOf<UCL_ConfirmationWidget> ConfirmationWidgetClass;
	
	UPROPERTY(Transient)
	TObjectPtr<UCL_ConfirmationDescriptor> Descriptor;

	//~ Begin UBlueprintAsyncActionBase
	virtual void Activate() override;
	//~ End UBlueprintAsyncActionBase
};
