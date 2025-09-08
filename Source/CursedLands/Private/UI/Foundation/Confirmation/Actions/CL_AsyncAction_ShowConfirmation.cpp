// Copyright Anton Vasserman, All Rights Reserved.

#include "UI/Foundation/Confirmation/Actions/CL_AsyncAction_ShowConfirmation.h"

#include "CL_GameplayTags.h"
#include "UI/CL_PrimaryGameLayoutWidget.h"
#include "UI/Foundation/Confirmation/CL_ConfirmationDescriptor.h"

UCL_AsyncAction_ShowConfirmation* UCL_AsyncAction_ShowConfirmation::ShowConfirmationYesNo(UObject* InWorldContextObject, TSubclassOf<UCL_ConfirmationWidget> InConfirmationWidgetClass, FText Title, FText Message)
{
	UCL_AsyncAction_ShowConfirmation* Action = NewObject<UCL_AsyncAction_ShowConfirmation>();
	Action->WorldContextObject = InWorldContextObject;
	Action->ConfirmationWidgetClass = InConfirmationWidgetClass;
	Action->Descriptor = UCL_ConfirmationDescriptor::CreateConfirmationYesNo(Title, Message);
	Action->RegisterWithGameInstance(InWorldContextObject);

	return Action;
}

void UCL_AsyncAction_ShowConfirmation::HandleConfirmationResult(ECL_ConfirmationWidgetResult ConfirmationResult)
{
	OnResult.Broadcast(ConfirmationResult);

	SetReadyToDestroy();
}

//~ Begin UBlueprintAsyncActionBase
void UCL_AsyncAction_ShowConfirmation::Activate()
{
	UCL_PrimaryGameLayoutWidget* PrimaryGameLayout = nullptr;
	
	if (WorldContextObject)
	{
		PrimaryGameLayout = UCL_PrimaryGameLayoutWidget::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject);
	}
	
	if (PrimaryGameLayout)
	{
		FCL_ConfirmationResultDelegate ResultCallback = FCL_ConfirmationResultDelegate::CreateUObject(this, &UCL_AsyncAction_ShowConfirmation::HandleConfirmationResult);

		PrimaryGameLayout->PushWidgetToLayerStack(CLGameplayTags::UI_Layer_Modal, ConfirmationWidgetClass, [this, ResultCallback](UCommonActivatableWidget& ConfirmationWidget)
		{
			UCL_ConfirmationWidget* ConfirmationWidgetPtr = CastChecked<UCL_ConfirmationWidget>(&ConfirmationWidget);
			ConfirmationWidgetPtr->Setup(Descriptor, ResultCallback);
		});
		
		return;
	}

	HandleConfirmationResult(ECL_ConfirmationWidgetResult::None);
}
//~ End UBlueprintAsyncActionBase
