// Copyright Anton Vasserman, All Rights Reserved.


#include "UI/Foundation/Confirmation/CL_ConfirmationWidget.h"

#if WITH_EDITOR
#include "Editor/WidgetCompilerLog.h"
#endif

#include "CommonBorder.h"
#include "CommonButtonBase.h"
#include "CommonRichTextBlock.h"
#include "CommonTextBlock.h"
#include "Components/DynamicEntryBox.h"
#include "ICommonInputModule.h"
#include "UI/Foundation/Buttons/CL_TextButtonBase.h"
#include "UI/Foundation/Confirmation/CL_ConfirmationDescriptor.h"

void UCL_ConfirmationWidget::Setup(UCL_ConfirmationDescriptor* Descriptor, FCL_ConfirmationResultDelegate ResultDelegate)
{
	Text_Title->SetText(Descriptor->Header);
	RichText_Description->SetText(Descriptor->Body);

	EntryBox_Buttons->Reset<UCommonButtonBase>([](UCommonButtonBase& Button)
	{
		Button.OnClicked().Clear();
	});

	for (const FConfirmationAction& Action : Descriptor->ButtonActions)
	{
		FDataTableRowHandle ActionRow;

		switch(Action.Result)
		{
		case ECL_ConfirmationWidgetResult::Yes:
			ActionRow = ICommonInputModule::GetSettings().GetDefaultClickAction();
			break;
		case ECL_ConfirmationWidgetResult::No:
			ActionRow = ICommonInputModule::GetSettings().GetDefaultBackAction();
			break;
		default:
			checkNoEntry()
			continue;
		}

		UCL_TextButtonBase* Button = EntryBox_Buttons->CreateEntry<UCL_TextButtonBase>();
		Button->SetTriggeringInputAction(ActionRow);
		Button->OnClicked().AddUObject(this, &ThisClass::CloseConfirmationWindow, Action.Result);
		Button->SetButtonText(Action.OptionalDisplayText);
	}
	
	OnResultCallback = ResultDelegate;
}

void UCL_ConfirmationWidget::CloseConfirmationWindow(ECL_ConfirmationWidgetResult Result)
{
	DeactivateWidget();
	OnResultCallback.ExecuteIfBound(Result);
}

FEventReply UCL_ConfirmationWidget::HandleTapToCloseZoneMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = FReply::Unhandled();

	if (MouseEvent.IsTouchEvent() || MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		CloseConfirmationWindow(ECL_ConfirmationWidgetResult::No);
		Reply.NativeReply = FReply::Handled();
	}

	return Reply;
}

//~ Begin UCommonActivatableWidget
#if WITH_EDITOR
void UCL_ConfirmationWidget::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	if (CancelAction.IsNull())
	{
		CompileLog.Error(FText::Format(FText::FromString(TEXT("{0} has unset property: CancelAction.")), FText::FromString(GetName())));
	}
}
#endif

void UCL_ConfirmationWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Border_TapToCloseZone->OnMouseButtonDownEvent.BindDynamic(this, &UCL_ConfirmationWidget::HandleTapToCloseZoneMouseButtonDown);
}
//~ End UCommonActivatableWidget
